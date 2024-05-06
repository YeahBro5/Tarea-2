#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 100

typedef struct {
  char id[100];
  char title[100];
  List *genres;
  char director[200];
  float rating;
  int year;
} Film;

// FUNCIONES DE UTILIDAD GENERAL
void mostrarMenuPrincipal();
void limpiarPantalla();
void presioneTeclaParaContinuar();
void pasarAFormato(char *cadena);
void introducirDecada(int *decada);
// Comparación
int is_equal_str(void *key1, void *key2);
int is_equal_int(void *key1, void *key2);

// FUNCIONES DE MANEJO DE DATOS
char **leer_linea_csv(FILE *archivo, char separador);
void guardarGeneros(Film *pelicula, char *generos);
void poblarMapaByDecada(Map *pelis_bydecada, Film *peli);
void poblarMapaByGenre(Map *pelis_bygenre, Film *peli);

// FUNCIONES DE LOS DISTINTOS MENÚS
void cargar_peliculas(Map *pelis_byid, Map *pelis_bygenre, Map *pelis_bydecada);
void buscar_por_id(Map *pelis_byid);
void buscarPorDecada(Map *pelis_bydecada);
void buscar_por_genre(Map *pelis_bygenre);
void buscarPorGeneroDecada(Map *pelis_bygenre, Map *pelis_bydecada);



int main() {
  char opcion;
  Map *pelis_byid = map_create(is_equal_str); // Mapa key = ID
  Map *pelis_bygenre = map_create(is_equal_str); // Mapa key = Género
  Map *pelis_bydecada = map_create(is_equal_str); // Mapa key = Década
  

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1': // Menú "Cargar Películas"
      cargar_peliculas(pelis_byid, pelis_bygenre, pelis_bydecada);
      break;
    case '2': // Menú "Buscar por id"
      buscar_por_id(pelis_byid);
      break;
    case '3': // Menú "Buscar por género"
      buscar_por_genre(pelis_bygenre);
      break;
    case '4': // Menú "Buscar por década"
      buscarPorDecada(pelis_bydecada);
      break;
    case '5': // Menú "Buscar por década y género"
      buscarPorGeneroDecada(pelis_bygenre, pelis_bydecada);
      break;
    case '6': // Salir
      puts("Saliendo de la base de datos..."); 
      break;
    default:
      puts("Opción no válida.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '6');

  // Liberación de memoria de los mapas
  free(pelis_byid);
  free(pelis_bygenre);
  free(pelis_bydecada);
  return 0;
}

// FUNCIONES

// Funciones de utilidad general

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Películas");
  puts("2) Buscar por id");
  puts("3) Buscar por género");
  puts("4) Buscar por década");
  puts("5) Buscar por década y género");
  puts("6) Salir");
}

void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

// Función encargada de la comparación de strings.
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

// Función encargada de la comparación de enteros.
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2;
}

// Función encargada de asegurar que se entregue el formato correcto en los géneros.
void pasarAFormato(char *cadena)
{
  for (int i = 0; cadena[i] != '\0'; i++)
    {
      if (i == 0)
      {
        cadena[i] = toupper(cadena[i]); // La primera letra se pasa a mayúsculas
      }
      else if (cadena[i] == '-') // En caso de que haya un guion, se pasa a mayúscula la letra siguiente
      {
        cadena[i + 1] = toupper(cadena[i + 1]); 
        i = i + 1;
      }
      else
      {
        cadena[i] = tolower(cadena[i]); // En cualquier otro caso, se pasan a minúsculas las letras
      }
    }
}

void introducirDecada(int *decada)
{
  getchar();
  while ((*decada % 10) != 0) { // Asegura que el número ingresado termine en 0
      printf("Ingrese una década (debe terminar en 0): ");
      while((scanf("%d", decada)) != 1) { // Verifica la entrada nuevamente
        printf("Ingrese una década válida: ");
          while (getchar() != '\n'); // Limpieza de Buffer
      }
  }

}



// FIN

// Funciones encargadas del manejo de datos

// Función encargada de leer cada línea del archivo entregado (Top1500.csv)
char **leer_linea_csv(FILE *archivo, char separador) {
  static char linea[MAX_LINE_LENGTH];
  static char *campos[MAX_FIELDS];
  char *ptr, *start;
  int idx = 0;

  if (fgets(linea, MAX_LINE_LENGTH, archivo) == NULL) {
    return NULL; // No hay más líneas para leer
  }

  ptr = start = linea;
  while (*ptr) {
    if (idx >= MAX_FIELDS - 1)
      break;

    if (*ptr == '\"') { // Inicio de un campo entrecomillado
      start = ++ptr;    // Saltar la comilla inicial
      while (*ptr && !(*ptr == '\"' && *(ptr + 1) == separador))
        ptr++;
    } else { // Campo sin comillas
      start = ptr;
      while (*ptr && *ptr != separador)
        ptr++;
    }

    if (*ptr) {
      *ptr = '\0'; // Reemplazar comilla final o separador por terminación
      ptr++;
      if (*ptr == separador)
        ptr++;
    }

    // Quitar comilla final si existe
    if (*(ptr - 2) == '\"') {
      *(ptr - 2) = '\0';
    }

    campos[idx++] = start;
  }

  campos[idx] = NULL; // Marcar el final del array
  return campos;
}

// Función encargada de separar los géneros leídos y guardarlos en la lista de la película.
void guardarGeneros(Film *pelicula, char *generos)
{
  pelicula->genres = list_create(); // Se crea una lista para almacenar los géneros de la película

  const char delimitador[] = ", "; // Delimitador de los Tokens
  char *genero = strtok(generos, delimitador); // Separación de los géneros
  while(genero != NULL) {
      while (*genero == ' ') genero++; // Se eliminan los espacios en blancos del género

      char *genero_copiado = strdup(genero); // Se realiza una copia del género
      if (genero_copiado != NULL) {
          list_pushBack(pelicula->genres, genero_copiado); // Se introduce la copia en la lista
      } else {

          printf("No se pudo realizar una copia del género correctamente");
      }

      genero = strtok(NULL, delimitador); // Se pasa al siguiente género
  }
}

// Función encargada de poblar el mapa, donde las claves serán sus respectivas décadas.
void poblarMapaByDecada(Map *pelis_bydecada, Film *peli)
{
  int *decada = (int *)malloc(sizeof(int)); // Se reserva memoria para la década.
  *decada = (peli->year / 10) * 10; // Se obtiene la década a través del año de la película.
  MapPair *pair = map_search(pelis_bydecada, decada); 
  if (pair == NULL) // En caso de no encontrar la década correspondiente..
  {
    List *lista = list_create(); // Se crea una lista
    list_pushBack(lista, peli); // Se agrega la respectiva película a la lista
    map_insert(pelis_bydecada, decada, lista); // Finalmente se agrega la lista con la película donde la clave será la década.
  }
  else{ // En caso de que se encuentre.. 
    list_pushBack(pair->value, peli); // Se agrega una nueva película a su lista correspondiente, según década.
  }
}

// Función encargada de poblar el mapa donde la clave serán los distintos géneros.
void poblarMapaByGenre(Map *pelis_bygenre, Film *peli)
{
  MapPair *pair;
  char *genre = (char *)list_first(peli->genres);
  while(genre !=  NULL) // Se recorre la lista de géneros de la película.
    {
      if ((pair = map_search(pelis_bygenre, genre)) != NULL)// En caso de que ya se encuentre el género en el mapa...

      {
        list_pushBack(pair->value, peli); // Se agrega la película en su respectivo género.
      }
      else{ // En caso contrario..
        List *lista = list_create(); // Se crea una lista  
        list_pushBack(lista, peli); // Se introduce la película correspondiente en la lista.
        map_insert(pelis_bygenre, genre, lista); // Se inserta la lista en el mapa donde la clave será el género.
      }

      genre = (char*)list_next(peli->genres);
    }
}

// FIN

// Funciones encargadas de los menús

void cargar_peliculas(Map *pelis_byid, Map *pelis_bygenre, Map *pelis_bydecada) {
  FILE *archivo = fopen("data/Top1500.csv", "r"); // Se abre el archivo
  if (archivo == NULL) {
    perror("Error al abrir el archivo");
    return;
  }

  char **campos;
  campos = leer_linea_csv(archivo, ','); // Se separa en campos cada información de la película.
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    Film *peli = (Film *)malloc(sizeof(Film)); // Reserva de memoria
    strcpy(peli->id, campos[1]); // Se llena su respectivo ID
    strcpy(peli->title, campos[5]); // Se llena su respectivo Titulo
    strcpy(peli->director, campos[14]); // Se llena su respectivo Director
    guardarGeneros(peli, campos[11]); // Se llena los generos en una lista.
    peli->year = atoi(campos[10]); // Se llena su respectivo Año
    peli->rating = atof(campos[8]); // Se llena su respectivo Rating.

    
    // POBLACIÓN DE CADA MAPA
    poblarMapaByDecada(pelis_bydecada, peli); 
    poblarMapaByGenre(pelis_bygenre, peli);
    map_insert(pelis_byid, peli->id, peli);
  }
  fclose(archivo);



  // se muestran las peliculas
  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    Film *peli = pair->value;
    printf("ID: %s, Título: %s, Año: %d, Rating: %.1f\n\n", peli->id, peli->title, peli->year, peli->rating);
    pair = map_next(pelis_byid);
  }
}

// Funcíon encargada del Menú "Buscar por id"
void buscar_por_id(Map *pelis_byid) {
  char id[10];
  printf("Ingrese el id de la película: ");
  scanf("%s", id);
  printf("\n");

  MapPair *pair = map_search(pelis_byid, id); // Se busca el respectivo ID en el mapa
  if (pair != NULL) { // En caso de que se encuentre, se muestra.
    Film *peli = pair->value;
    char *listaGeneros = (char *)list_first(peli->genres);
    printf("Título: %s, Año: %d, Géneros: ", peli->title, peli->year);
    while (listaGeneros != NULL) // Bucle para mostrar todos los géneros de la película
      {
        printf("%s", listaGeneros);
        listaGeneros = (char *)list_next(peli->genres);
        if (listaGeneros != NULL)
        {
          printf(", ");
        }
      }
    printf("\n\n");
  } else // Caso contrario..
    printf("La película con id %s no existe\n\n", id);
}

// Funcíon encargada del menú "Buscar por género"
void buscar_por_genre(Map *pelis_bygenre) {
  char genero[50];
  printf("Ingrese el género que desea buscar: ");
  scanf("%s", genero);
  pasarAFormato(genero); // Se llama a la función para asegurar que el formato sea el correcto.
  printf("\n");

  MapPair *pair = map_search(pelis_bygenre, genero); // Se busca el respectivo género en el mapa.
  if (pair != NULL) {

    Film *peli = (Film *)list_first(pair->value);
    while(peli != NULL) // En caso de encontrarlo, se muestra su respectiva lista de películas.
      {
        char *listaGeneros = (char *)list_first(peli->genres);
        printf("ID: %s, Título: %s, Año: %d, Géneros: ", peli->id, peli->title, peli->year);
        while (listaGeneros != NULL) // Bucle para mostrar todos los géneros de la película
          {
            printf("%s", listaGeneros);
            listaGeneros = (char *)list_next(peli->genres);
            if (listaGeneros != NULL)
            {
              printf(", ");
            }

          }
        printf("\n\n");

      peli = (Film *)list_next(pair->value);

      }

  }
  else{ // Caso contrario..
    printf("No existen películas con el género %s\n\n", genero);
  }
}

// Función encargada del Menú "Buscar por década"
void buscarPorDecada(Map *pelis_bydecada)
{
  int decada = 1; // Se inicializa la década para así poder ser manipulada.
  introducirDecada(&decada); // Función encargada de solicitar la década correctamente.
  printf("\n");

  MapPair *pair = map_search(pelis_bydecada, &decada); // Se busca la respectiva década en el mapa
  if (pair != NULL) {

    Film *peli = (Film *)list_first(pair->value);
    while(peli != NULL) // En caso de encontrarlo, se muestra su respectiva lista de películas.
      {
        printf("ID: %s, Título: %s, Año: %d\n\n", peli->id, peli->title, peli->year);

        peli = (Film *)list_next(pair->value);
      }

  } else // Caso contrario..
    printf("No existen películas de la década %d\n", decada);

  printf("\n");

}

// Función encargada del menú "Buscar por década y género"
void buscarPorGeneroDecada( Map *pelis_bygenre, Map *pelis_bydecada){
  char genero[50];
  int decada = 1; // Se inicializa la década para así poder ser manipulada.
  int decadaPeli; 
  int flag = 0;// Variable encargada de determinar si se encontró una película con los parámetros establecidos.
  getchar();
  printf("Ingrese el género que desea buscar: ");
  scanf("%s", genero);
  pasarAFormato(genero);
  introducirDecada(&decada);
  printf("\n");

  MapPair *pair = map_search(pelis_bygenre, genero); // Se busca el género en su respectivo mapa.
  if (pair != NULL) { // En caso de encontrarlo..

    Film *peli = (Film *)list_first(pair->value); // Se recorren las películas del respectivo género.
    while(peli != NULL)
      {
        decadaPeli = (peli->year / 10) * 10; // Se obtiene la década de cada película
        char *listaGeneros = (char *)list_first(peli->genres);
        if (decadaPeli == decada) // Cuando se encuentre una película de la misma década..
        {
          flag = 1; // Se actualiza la variable para "avisar" que ya se encontró al menos una película.
          printf("ID: %s, Título: %s, Año: %d, Géneros: ", peli->id, peli->title, peli->year); // Se muestra dicha película
          while (listaGeneros != NULL) // Bucle para mostrar los géneros de dicha película
            {
              printf("%s", listaGeneros);
              listaGeneros = (char *)list_next(peli->genres);
              if (listaGeneros != NULL)
              {
                printf(", ");
              }

            }
          printf("\n\n");
        }

        peli = (Film *)list_next(pair->value);

      }

  } 

  if (flag == 0) // En caso de que el flag sea igual a 0, significa que no se encontró ninguna película.
    {
      printf("No existen películas de %s Y de la década de %d\n\n", genero, decada);
    }
}
