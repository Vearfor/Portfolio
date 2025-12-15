# Soy **Enrique Rocafull Ortiz**.

### Perdonad, pero tengo el portfolio en construccion .. poco a poco ..

  **Programador** o **Ingeniero de Desarrollo** (lo que mas bonito os suene) Senior con más de 20 años de experiencia en el desarrollo de aplicaciones en **C, C++ y C#**. Profesional titulado **Licenciado en Informática** por la **Universidad Politécnica de Madrid**.
  
  Me apasiona programar, y me apasionaría poder hacerlo en el mundo de los VideoJuegos.
  He intentado entrar en el mundo del desarrollo de los VideoJuegos desde hace mas de 20 años.
  Pero no creía en principio que mis habilidades pudieran cubrir las necesidades actuales de los juegos contemporaneos.
  Pensaba que tendría que tener conociminetos profundos en el desarrollo con motores gráficos estilo *Unity* o *UnReal*, que no tendría la experiencia y capacidad demostrada para poder trabajar en ello.

  - La base de este Portfolio fue el repositorio que construí para responder a una serie de ejercicios que la empresa de videojuegos **BAMTANG** proponía como tests para poder trabajar con ellos. Los ejercicios propuestos, fáciles o difíciles, eran asequibles con mis capacidades. Y pensé que para entrar en el mundo de los videojuegos, con mis capacidades de programación, no tendría que estar aprendiendo *Unity* o *UnReal*. Así que probé a realizarlos, y de aquí salió un repositorio para mostrar a la empresa de la cual nunca tube respuesta.
  - Se que en esta profesión tenemos que seguir aprendiendo y evolucionando. Eso es lo divertido. En realidad, ¿qué profesiones no son así? Siempre tenemos que evolucionar y seguir aprendiendo, y seguir mejorando.
  - Me encantaría trabajar en el mundo de los videojuegos. Me apasionan. Quiero aprender, y seguir mejorando.
  - Si pudiera tener la frase que os convenciera ..., que más os puedo decir.
  - Y antes que volver a probar en un nuevo trabajo que no me llene, que no me satisfaga, quiero probar de una vez por todas a trabajar en/con VideoJuegos.

### Esto sigue siendo un Borrador ###
- 

**Curriculum**
  - Carpeta en la que mantendremos los curriculums actualizados.

**TestBamTang**
  - Carpeta en la que tenemos los ejercicios resueltos de los propuestos por **Bamtang**.
  - El código estará evolucionado del enlace público que deje para BamTang: ***https://github.com/Vearfor/TestBamTang.git***. Aunque básicamnete es el mismo.
    + **TestBamTang.sln**:  Solucion original de Visual Studio 2019, ya la hemos pasado a 2022. Y estos son las carpetas de los proyectos:
      - **BamTang01**  Cypher

        ![Cypher](/TestBamTang/Imagenes/BamTang01.png)
  
      - **BamTang02**  Laberinto

        ![Cypher](/TestBamTang/Imagenes/BamTang02.png)
  
      - **BamTang03**  Laberinto 2D
        + Tenemos una 'demo' preparada que se lanza con:    **bamtang03.exe** <*size*> **Demo**   Siendo <size> la dimensión del laberinto. Se puede poner por ejemplo: **bamtan03 21 Demo**
        + Los hemos limitado a 25, porque cuando se mostraba en la consola el ocupaba toda la pantalla.

        ![Cypher](/TestBamTang/Imagenes/BamTang03.png)
  
      - **BamTang05**  Laberinto 3D
        + Al igual que en BamTang03, hemos dejado tambien la misma opción de 'demo'. Por ejemplo: **bamtan05 21 Demo**
        + Se han quedado con la limitación de 25.
        + Hemos utilizado la misma carpeta que **'comun/src'** que luego utiliza **'ConsoleApplication1'**.
      
          ![Cypher](/TestBamTang/Imagenes/BamTang05.png)
  
      - ***ConsoleApplication1*** para certificar pruebas de concepto.
        + Hemos utilizado la misma carpeta que **'comun/src'** que luego utiliza **'Bamtan05'**.
        + Todo esto era preparatorio para la construcción de la libreria **'suat'**, que utilizaba el mismo codigo.
      
      - **suat**.      Proyecto para la librería dinámica **'suat.dll'**.
        + He agrupado en ella todo el codigo que he ido utilizando en todos los proyectos.
        + Es casi un mini motor de videojuegos ya que tiene todo el conocimiento que he ido agrupando a lo largo de los años.
        + Es el mismo código que luego esta en la carpeta **'comun/src'**.
  
      - **BamTang04**  Físicas. ***Y tengo que dar por terminado el trabajo, porque sino estaría en un proceso de mejora interminable***.
        + Ya utiliza la librería **'suat'**.
      
          ![Cypher](/TestBamTang/Imagenes/BamTang04.png)
          
      Insisto, **BamTang05** y **ConsoleApplication1** matienen en comun la carpeta **comun/src**, el mismo código que ya hemos metido en **suat.dll**.
      Ha sido un proceso de mejora continua que culmina al final en esta 'dll'.
      He puesto el trabajo de Físicas, **BamTamg04**, al final, porque ha sido el último que he realizado.
      Todo está para que funcione con el **Visual Studio 2019**. Y ya la hemmos pasado a **Visual Studio 2022**.
      Y todo esta agrupado en una única solución: **'TestBamTang.sln'**.

Espero que os aproveche.


Hasta pronto.

