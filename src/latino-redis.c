/*
The MIT License (MIT)

Copyright (c) Latino - Lenguaje de Programacion

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */

#include <stdlib.h>

#ifdef _WIN32
#define LATINO_BUILD_AS_DLL
#endif

#define LATINO_LIB

#include "hiredis.h"
#include "latino.h"

#define LIB_REDIS_NAME "redis"

void redis_conectar(lat_mv *mv) {
  lat_objeto *puerto = latC_desapilar(mv);
  lat_objeto *servidor = latC_desapilar(mv);
  redisContext *redis;
  const char *servidor2 = latC_checar_cadena(mv, servidor);
  int puerto2 = latC_checar_numerico(mv, puerto);
  struct timeval timeout = {1, 500000}; // 1.5 segundos
  redis = redisConnectWithTimeout(servidor2, puerto2, timeout);
  if (redis == NULL || redis->err) {
    if (redis) {
      redisFree(redis);
      latC_error(mv, "Error en conexion");
    } else {
      latC_error(mv, "Error en contexto redis");
    }
  }
  // encapsulo el dato (struct) de redis
  lat_objeto *cref = latC_crear_cdato(mv, (void *)redis);
  latC_apilar(mv, cref);
}

void redis_desconectar(lat_mv *mv) {
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisFree(conexion);
}

void redis_ping(lat_mv *mv) {
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisReply *respuesta;
  respuesta = redisCommand(conexion, "PING");
  if (respuesta->str) {
    lat_objeto *tmp = latC_crear_cadena(mv, respuesta->str);
    latC_apilar(mv, tmp);
  } else {
    latC_apilar(mv, latO_falso);
  }
  freeReplyObject(respuesta);
}

void redis_asignar(lat_mv *mv) {
  lat_objeto *cadena = latC_desapilar(mv);
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  char *str = latC_checar_cadena(mv, cadena);
  redisReply *respuesta;
  respuesta =
      redisCommand(conexion, "GETSET %s %s", latC_checar_cadena(mv, hash),
                   latC_checar_cadena(mv, cadena));
  if (!respuesta->str) {
    latC_apilar(mv, latC_crear_cadena(mv, str));
  } else {
    latC_apilar(mv, latO_falso);
  }
  freeReplyObject(respuesta);
}

void redis_hasignar(lat_mv *mv) {
  lat_objeto *cadena = latC_desapilar(mv);
  lat_objeto *llave = latC_desapilar(mv);
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisReply *respuesta;
  respuesta = redisCommand(
      conexion, "HSET %s %s %s", latC_checar_cadena(mv, hash),
      latC_checar_cadena(mv, llave), latC_checar_cadena(mv, cadena));
  freeReplyObject(respuesta);
}

void redis_obtener(lat_mv *mv) {
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisReply *respuesta;
  respuesta = redisCommand(conexion, "GET %s", latC_checar_cadena(mv, hash));
  lat_objeto *tmp;
  if (respuesta->str) {
    tmp = latC_crear_cadena(mv, respuesta->str);
  } else {
    tmp = latO_nulo;
  }
  latC_apilar(mv, tmp);
  freeReplyObject(respuesta);
}

void redis_hobtener(lat_mv *mv) {
  lat_objeto *llave = latC_desapilar(mv);
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisReply *respuesta;
  respuesta = redisCommand(conexion, "HGET %s %s", latC_checar_cadena(mv, hash),
                           latC_checar_cadena(mv, llave));
  lat_objeto *tmp;
  if (respuesta->str) {
    tmp = latC_crear_cadena(mv, respuesta->str);
  } else {
    tmp = latO_nulo;
  }
  latC_apilar(mv, tmp);
  freeReplyObject(respuesta);
}

void redis_borrar(lat_mv *mv) {
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisReply *respuesta;
  respuesta = redisCommand(conexion, "DEL %s", latC_checar_cadena(mv, hash));
  if (!respuesta->integer) {
    latC_apilar(mv, latO_falso);
  } else {
    latC_apilar(mv, latO_verdadero);
  }
  freeReplyObject(respuesta);
}

void redis_hborrar(lat_mv *mv) {
  lat_objeto *llave = latC_desapilar(mv);
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisReply *respuesta;
  respuesta = redisCommand(conexion, "HDEL %s %s", latC_checar_cadena(mv, hash),
                           latC_checar_cadena(mv, llave));
  if (!respuesta->integer) {
    latC_apilar(mv, latO_falso);
  } else {
    latC_apilar(mv, latO_verdadero);
  }
  freeReplyObject(respuesta);
}

void redis_aumentar(lat_mv *mv) {
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisReply *respuesta;
  respuesta = redisCommand(conexion, "INCR %s", latC_checar_cadena(mv, hash));
  if (!respuesta->integer) {
    latC_apilar(mv, latO_falso);
  } else {
    lat_objeto *tmp = latC_crear_numerico(mv, (double)respuesta->integer);
    latC_apilar(mv, tmp);
  }
  freeReplyObject(respuesta);
}

void redis_haumentar(lat_mv *mv) {
  lat_objeto *llave = latC_desapilar(mv);
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisReply *respuesta;
  respuesta =
      redisCommand(conexion, "HINCRBY %s %s 1", latC_checar_cadena(mv, hash),
                   latC_checar_cadena(mv, llave));
  if (!respuesta->integer) {
    latC_apilar(mv, latO_falso);
  } else {
    lat_objeto *tmp = latC_crear_numerico(mv, (double)respuesta->integer);
    latC_apilar(mv, tmp);
  }
  freeReplyObject(respuesta);
}

void redis_incrementar(lat_mv *mv) {
  lat_objeto *numero = latC_desapilar(mv);
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisReply *respuesta;
  long int numerico = latC_checar_numerico(mv, numero);
  respuesta = redisCommand(conexion, "INCRBY %s %i",
                           latC_checar_cadena(mv, hash), numerico);
  if (!respuesta->integer) {
    latC_apilar(mv, latO_falso);
  } else {
    lat_objeto *tmp = latC_crear_numerico(mv, respuesta->integer);
    latC_apilar(mv, tmp);
  }
  freeReplyObject(respuesta);
}

void redis_hincrementar(lat_mv *mv) {
  lat_objeto *numero = latC_desapilar(mv);
  lat_objeto *llave = latC_desapilar(mv);
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisReply *respuesta;
  long int numerico = latC_checar_numerico(mv, numero);
  respuesta =
      redisCommand(conexion, "HINCRBY %s %s %i", latC_checar_cadena(mv, hash),
                   latC_checar_cadena(mv, llave), numerico);
  if (!respuesta->integer) {
    latC_apilar(mv, latO_falso);
  } else {
    lat_objeto *tmp = latC_crear_numerico(mv, respuesta->integer);
    latC_apilar(mv, tmp);
  }
  freeReplyObject(respuesta);
}

void redis_hllaves(lat_mv *mv) {
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  lat_objeto *elementos = latC_crear_lista(mv, latL_crear(mv));
  redisReply *respuesta;
  respuesta = redisCommand(conexion, "HKEYS %s", latC_checar_cadena(mv, hash));
  if (respuesta->elements) {
    for (int i = 0; i < respuesta->elements; i++) {
      latL_agregar(mv, latC_checar_lista(mv, elementos),
                   latC_crear_cadena(mv, respuesta->element[i]->str));
    };
    latC_apilar(mv, elementos);
  } else {
    latC_apilar(mv, latO_nulo);
  };
  freeReplyObject(respuesta);
}

void redis_llaves(lat_mv *mv) {
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  lat_objeto *elementos = latC_crear_lista(mv, latL_crear(mv));
  redisReply *respuesta;
  respuesta = redisCommand(conexion, "KEYS %s", latC_checar_cadena(mv, hash));
  if (respuesta->elements) {
    for (int i = 0; i < respuesta->elements; i++) {
      latL_agregar(mv, latC_checar_lista(mv, elementos),
                   latC_crear_cadena(mv, respuesta->element[i]->str));
    };
    latC_apilar(mv, elementos);
  } else {
    latC_apilar(mv, latO_nulo);
  };
  freeReplyObject(respuesta);
}

void redis_smiembros(lat_mv *mv) {
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  lat_objeto *elementos = latC_crear_lista(mv, latL_crear(mv));
  redisReply *respuesta;
  respuesta =
      redisCommand(conexion, "SMEMBERS %s", latC_checar_cadena(mv, hash));
  if (respuesta->elements) {
    for (int i = 0; i < respuesta->elements; i++) {
      latL_agregar(mv, latC_checar_lista(mv, elementos),
                   latC_crear_cadena(mv, respuesta->element[i]->str));
    };
    latC_apilar(mv, elementos);
  } else {
    latC_apilar(mv, latO_nulo);
  };
  freeReplyObject(respuesta);
}

void redis_sagregar(lat_mv *mv) {
  lat_objeto *llave = latC_desapilar(mv);
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisReply *respuesta;
  respuesta = redisCommand(conexion, "SADD %s %s", latC_checar_cadena(mv, hash),
                           latC_checar_cadena(mv, llave));
  freeReplyObject(respuesta);
  if (!respuesta->integer) {
    latC_apilar(mv, latO_falso);
  } else {
    latC_apilar(mv, latO_verdadero);
  }
}

void redis_sborrar(lat_mv *mv) {
  lat_objeto *llave = latC_desapilar(mv);
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisReply *respuesta;
  respuesta = redisCommand(conexion, "SREM %s %s", latC_checar_cadena(mv, hash),
                           latC_checar_cadena(mv, llave));
  freeReplyObject(respuesta);
  if (!respuesta->integer) {
    latC_apilar(mv, latO_falso);
  } else {
    latC_apilar(mv, latO_verdadero);
  }
}

void redis_sesmiembro(lat_mv *mv) {
  lat_objeto *llave = latC_desapilar(mv);
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisReply *respuesta;
  respuesta =
      redisCommand(conexion, "SISMEMBER %s %s", latC_checar_cadena(mv, hash),
                   latC_checar_cadena(mv, llave));
  freeReplyObject(respuesta);
  if (!respuesta->integer) {
    latC_apilar(mv, latO_falso);
  } else {
    latC_apilar(mv, latO_verdadero);
  }
}

void redis_expirar(lat_mv *mv) {
  lat_objeto *tiempo = latC_desapilar(mv);
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  int num = latC_checar_numerico(mv, tiempo);
  redisReply *respuesta;
  respuesta =
      redisCommand(conexion, "EXPIRE %s %d", latC_checar_cadena(mv, hash), num);
  freeReplyObject(respuesta);
  if (!respuesta->integer) {
    latC_apilar(mv, latO_falso);
  } else {
    latC_apilar(mv, latO_verdadero);
  }
}

void redis_adjuntar(lat_mv *mv) {
  lat_objeto *string = latC_desapilar(mv);
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisReply *respuesta;
  respuesta =
      redisCommand(conexion, "APPEND %s %s", latC_checar_cadena(mv, hash),
                   latC_checar_cadena(mv, string));
  freeReplyObject(respuesta);
  if (!respuesta->integer) {
    latC_apilar(mv, latO_falso);
  } else {
    latC_apilar(mv, latO_verdadero);
  }
}

void redis_setex(lat_mv *mv) {
  lat_objeto *tiempo = latC_desapilar(mv);
  lat_objeto *cadena = latC_desapilar(mv);
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  int num = latC_checar_numerico(mv, tiempo);
  redisReply *respuesta;
  respuesta =
      redisCommand(conexion, "SETEX %s %d %s", latC_checar_cadena(mv, hash),
                   num, latC_checar_cadena(mv, cadena));
  freeReplyObject(respuesta);
  if (!respuesta->str) {
    latC_apilar(mv, latO_falso);
  } else {
    latC_apilar(mv, latO_verdadero);
  }
}

void redis_tiempo(lat_mv *mv) {
  lat_objeto *hash = latC_desapilar(mv);
  lat_objeto *o = latC_desapilar(mv);
  redisContext *conexion = latC_checar_cptr(mv, o);
  redisReply *respuesta;
  respuesta = redisCommand(conexion, "TTL %s", latC_checar_cadena(mv, hash));
  freeReplyObject(respuesta);
  if (respuesta->integer) {
    if (respuesta->integer == -2) {
      latC_apilar(mv, latO_falso);
    } else if (respuesta->integer == -1) {
      latC_apilar(mv, latO_nulo);
    } else {
      latC_apilar(mv, latC_crear_numerico(mv, respuesta->integer));
    }
  } else {
    latC_apilar(mv, latC_crear_numerico(mv, 0));
  }
}

static const lat_CReg libredis[] = {{"conectar", redis_conectar, 2},
                                    {"desconectar", redis_desconectar, 1},
                                    {"ping", redis_ping, 1},
                                    {"asignar", redis_asignar, 3},
                                    {"hasignar", redis_hasignar, 4},
                                    {"obtener", redis_obtener, 2},
                                    {"hobtener", redis_hobtener, 3},
                                    {"borrar", redis_borrar, 2},
                                    {"hborrar", redis_hborrar, 3},
                                    {"aumentar", redis_aumentar, 2},
                                    {"haumentar", redis_haumentar, 3},
                                    {"incrementar", redis_incrementar, 3},
                                    {"hincrementar", redis_hincrementar, 4},
                                    {"hllaves", redis_hllaves, 2},
                                    {"llaves", redis_llaves, 2},
                                    {"smiembros", redis_smiembros, 2},
                                    {"sagregar", redis_sagregar, 3},
                                    {"sborrar", redis_sborrar, 3},
                                    {"sesmiembro", redis_sesmiembro, 3},
                                    {"expirar", redis_expirar, 3},
                                    {"adjuntar", redis_adjuntar, 3},
                                    {"setex", redis_setex, 4},
                                    {"tiempo", redis_tiempo, 2},
                                    {NULL, NULL}};

LATINO_API void latC_abrir_liblatino_redis(lat_mv *mv) {
  latC_abrir_liblatino(mv, LIB_REDIS_NAME, libredis);
}
