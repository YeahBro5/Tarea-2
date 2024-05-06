
## Base de Datos de Películas
## Descripción:

Esta base de datos permite que el usuario pueda cargar todas las películas a través de un archivo CSV con su respectiva información (ID, Título, géneros, director, rating y año) e ir gestionándolo a través de distintos parámetros. El usuario podrá buscar películas por su respectivo ID o género o década, como también podrá buscar tanto por década Y género.

## Cómo compilar y ejecutar
Para mayor simplicidad al momento de probar y ejecutar el codigo se utilizará la plataforma [Repl.it](http://repl.it/), los pasos a seguir son:

1. Visita [Repl.it](https://repl.it/).
2. Crea una nueva cuenta o inicia sesión si ya tienes una.
3. Una vez en tu dashboard, selecciona "Create Repl" y elige "Import from GitHub".
4. Pega la URL del repositorio: `URL REPOSITORIO`.
5. [Repl.it](http://repl.it/) clonará el repositorio y preparará un entorno de ejecución.
6. Configure el comando de compilación con `COMANDO DE COMPILACION` y el comando run con (`COMANDO RUN`).
7. Presiona el botón "Run" para compilar y ejecutar la aplicación.

## Funcionalidades
## Funcionando correctamente:
* `Cargar Películas`: Se lee el archivo CSV y se almacen las distintas peliculas en la base de datos.
* `Buscar por id`: Se muestra la película correspondiente al ID introducido.
* `Buscar por género`: Muestra todas las películas que tengan un género en especifico
* `Buscar por década`: Muestra todas las películas que hayan salido en una década en específico
* `Buscar por década y género`: Muestra todas las películas que tengan tanto una década como género en específico

## Problemas conocidos:
* A través de Repl.it se dificulta el desplazarse por todas las películas mostradas en un menú

## A mejorar:
* Mejorar la estética del sistema para que sea mas atractivo para el usuario.

## Ejemplo de uso:
**Paso 1: Cargar Películas**
Se comienza cargando las películas a través de un arhivo CSV
````
Opción seleccionada: 1) Cargar Películas

ID: tt0018756, Título: Champagne, Año: 1928, Rating: 5.4

ID: tt0143145, Título: The World Is Not Enough, Año: 1999, Rating: 6.4

ID: tt0040982, Título: You Gotta Stay Happy, Año: 1948, Rating: 6.7
...
...
````
"El sistema carga todas las películas y las muestra con parte de la información (ID, TÍTULO, AÑO, RATING)."

**Paso 2: Buscar por id**

Despues de cargar todas las peliculas, el usuario desea buscar una pelicula en específico a traves del ID para asi revisar su información.
````
Opción seleccionada: 2) Buscar por id

Ingrese el id de la película: tt1375666

Título: Inception, Año: 2010, Géneros: Action, Adventure, Sci-Fi, Thriller

````
El sistema muestra la película correspondiente al ID con su respectiva información

**Paso 3: Buscar por década**

El usuario ahora desea ver todas las películas relacionadas con un género en específico.
````
Opción seleccionada: 3) Buscar por género

Ingrese el género que desea buscar: sci-fi

ID: tt0080684, Título: Star Wars: Episode V - The Empire Strikes Back, Año: 1980, Géneros: Action, Adventure, Fantasy, Sci-Fi

ID: tt0076759, Título: Star Wars, Año: 1977, Géneros: Action, Adventure, Fantasy, Sci-Fi

ID: tt0066921, Título: A Clockwork Orange, Año: 1971, Géneros: Crime, Sci-Fi
...
...

````
El sistema automaticamente mostrará todas las peliculas que contengan el genero seleccionado, en este caso, Sci-Fi.

**Paso 5: Buscar por década**

Ahora el usuario desea el buscar todas las películas que hayan salido en la decada de 1990.
````
Opción seleccionada: 4) Buscar por década
Ingrese una década (debe terminar en 0): 1990

ID: tt0110912, Título: Pulp Fiction, Año: 1994

ID: tt0108052, Título: Schindler's List, Año: 1993

ID: tt0109830, Título: Forrest Gump, Año: 1994
...
...
````
El sistema ahora mostrará todas las películas de la decada de 1990.

**Paso 6: Buscar por década y género**

Una vez buscado tanto por género y década decide que quiere juntar estos 2 parametros, por lo tanto buscará una película que haya salido durante la decada de 1990 y que sea de Sci-Fi.
````
Opción seleccionada: 5) Buscar por década y género
Ingrese el género que desea buscar: Sci-Fi
Ingrese una década (debe terminar en 0): 1990

ID: tt0107290, Título: Jurassic Park, Año: 1993, Géneros: Action, Adventure, Sci-Fi, Thriller

ID: tt0114746, Título: Twelve Monkeys, Año: 1995, Géneros: Mystery, Sci-Fi, Thriller

ID: tt0103064, Título: Terminator 2: Judgment Day, Año: 1991, Géneros: Action, Adventure, Sci-Fi
...
...

````
Finalmente, el sistema mostrará todas las peliculas que cumplan con los 2 parámetros descritos.