### latino-redis
Librería dinámica para manejo de redis en [latino](https://github.com/primitivorm/latino)

#### Prerequisitos
Tener instalado [latino](https://github.com/primitivorm/latino)
Tener instalado [redis-server](http://packages.ubuntu.com/search?keywords=redis-server)

### Instalación
##### linux
```
git clone https://github.com/primitivorm/latino-redis
cd latino-redis
git submodule update --init --recursive
sudo chmod +x instalar.sh
sudo bash instalar.sh
```

#### Dependecias
[hiredis](https://github.com/redis/hiredis)
[redis-server](http://packages.ubuntu.com/search?keywords=redis-server)

##### Uso de esta librería en código latino
```
/*
#instalar redis-server en ubuntu
sudo apt-get install redis-server
#start service
sudo systemctl start redis
*/

//necesario para agregar la libreria dinamica
incluir("redis")
//se conecta con el servicio de redis corriendo en el puerto 6379
conexion = redis.conectar("127.0.0.1", 6379)
# Redis de 1 llave
poner("#### Valores de 1 llave ####")
//asignamos una clave
redis.asignar(conexion, "Latino", "Hola amigos latinos")

//asignamos un entero a otra llave
redis.asignar(conexion, "entero", "0")

// obtenemos los valores Redis de 1 llave
cadena = redis.obtener(conexion, "Latino")
entero  = redis.obtener(conexion, "entero")

// Devolvemos los valores Redis de 1 llave
poner("El valor de la cadena es:\t"..cadena)
poner("El valor del entero es:\t\t"..entero)

// le aumentamos 1 al entero y lo declaramos
nuevo_valor = redis.aumentar(conexion, "entero")
poner("Ahora el entero vale:\t\t"..nuevo_valor)
// le incrementamos el valor que desees, en este caso es 5
nuevo_valor = redis.incrementar(conexion, "entero", 5)
poner("Se le incrementa 5 al entero:\t"..nuevo_valor)

// borramos las llaves
redis.borrar(conexion, "Latino")
redis.borrar(conexion, "entero")

//////////////////\\\\\\\\\\\\\\\\\\\\

# Redis de 2 llaves
poner("\n#### Valores de 2 llaves ####")
//asignamos una clave
redis.hasignar(conexion, "latino", "saludo", "Hola amigo Latino")

//asignamos un entero a otra llave
redis.hasignar(conexion, "entero", "valor", "0")

// obtenemos los valores Redis de 1 llave
cadena = redis.hobtener(conexion, "latino", "saludo")
entero  = redis.hobtener(conexion, "entero", "valor")

// Devolvemos los valores Redis de 1 llave
poner("El valor de la cadena es:\t"..cadena)
poner("El valor del entero es:\t\t"..entero)

// le aumentamos 1 al entero y lo declaramos
nuevo_valor = redis.haumentar(conexion, "entero", "valor")
poner("Ahora el entero vale:\t\t"..nuevo_valor)
// le incrementamos el valor que desees, en este caso es 5
nuevo_valor = redis.hincrementar(conexion, "entero", "valor", 5)
poner("Se le incrementa 5 al entero:\t"..nuevo_valor)

// borramos las llaves
redis.hborrar(conexion, "latino", "saludo")
redis.hborrar(conexion, "entero", "valor")

// Prueba del servicio Redis
poner("\n### Devolución del ping ###")
ping = redis.ping(conexion)
poner("PING:\t\t"..ping)

// terminamos la conexión
redis.desconectar(conexion)
```

##### Cualquier aportación o sugerencia es bienvenida
