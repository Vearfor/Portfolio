Presentación
  - Soy **Enrique Rocafull**.
  
  Me apasiona programar, y me apasionaría poder hacerlo en el mundo de los VideoJuegos.
  He intentado entrar en el mundo del desarrollo de los VideoJuegos desde hace mas de 20 años.
  Pero no creía en principio que mis habilidades pudieran cubrir las necesidades actuales de los juegos contemporaneos.
  Pensaba que tendría que tener conociminetos profundos en el desarrollo con motores gráficos estilo Unity o UnReal, que no tendría la experiencia y capacidad demostrada para poder trabajar en ello.
  
  - Pero cuando he visto los ejercicios propuestos en esta empresa vuestra, **BAMTANG**, he visto que era capaz de hacerlos todos ellos, y que mis capacidades actuales podrían superar las espectativas y podría estar cumpliendo mi sueño.
  - Así que me he animado a probar.
Reconozco que no tengo todo el conocimiento para poder resolver todos los problemas.
Reconozco que las soluciones que podría tener ahora las tengo obsoletas.
Tengo que aprender, mejorar, seguir en un proceso de cambio profundo.
Pero que queréis sino, en realidad es lo que he hecho durante toda mi vida profesional
Me encantaría. Me apasionan. Quiero aprender, y seguir mejorando.
Si pudiera tener la frase que os convenciera ..., que más os puedo decir.
  
  - Y antes que volver a probar en un nuevo trabajo que no me llene, que no me satisfaga, quiero probar de una vez por todas a trabajar en/con VideoJuegos.

  - He agregado mi Cv en la carpeta de Curriculum.


**TestBamTang.sln**:  Solucion de Visual Studio 2019. Y estos son las carpetas de los proyectos:
- **BamTang01**  Cypher
  
- **BamTang02**  Laberinto
  
- **BamTang03**  Laberinto 2D
  + Tenemos una 'demo' preparada que se lanza con:    **bamtang03.exe** <*size*> **Demo**   Siendo <size> la dimensión del laberinto. Se puede poner por ejemplo: **bamtan03 21 Demo**
  + Los hemos limitado a 25, porque cuando se mostraba en la consola el ocupaba toda la pantalla.
  
- **BamTang05**  Laberinto 3D
  + Al igual que en BamTang03, hemos dejado tambien la misma opción de 'demo'. Por ejemplo: **bamtan05 21 Demo**
  + Se han quedado con la limitación de 25.
  + Hemos utilizado la misma carpeta que **'comun/src'** que luego utiliza **'ConsoleApplication1'**.
  
- ***ConsoleApplication1*** para certificar pruebas de concepto.
  + Hemos utilizado la misma carpeta que **'comun/src'** que luego utiliza **'Bamtan05'**.
  + Todo esto era preparatorio para la construcción de la libreria **'suat'**, que utilizaba el mismo codigo.

- **suat**.      Proyecto para la librería dinámica **'suat.dll'**.
  + He agrupado en ella todo el codigo que he ido utilizando en todos los proyectos.
  + Es casi un mini motor de videojuegos ya que tiene todo el conocimiento que he ido agrupando a lo largo de los años.
  + Es el mismo código que luego esta en la carpeta **'comun/src'**.
  
- **BamTang04**  Físicas. ***Y tengo que dar por terminado el trabajo, porque sino estaría en un proceso de mejora interminable***.
  + Ya utiliza la librería **'suat'**.
 
Insisto, **BamTang05** y **ConsoleApplication1** matienen en comun la carpeta **comun/src**, el mismo código que ya hemos metido en **suat.dll**.
Ha sido un proceso de mejora continua que culmina al final en esta 'dll'.
He puesto el trabajo de Físicas, **BamTamg04**, al final, porque ha sido el último que he realizado.
Todo está para que funciones con el **Visual Studio 2019**.
Y todo esta agrupado en una única solución: **'TestBamTang.sln'**.
