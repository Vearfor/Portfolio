Presentacion
  - Soy **Enrique Rocafull**.
  
  Me apasiona programar, y me apasiona poder hacerlo en el mundo de los Video Juegos.
  He intentado entrar en el mundo del desarrollo de los Video Juegos desde hace mas de 20 años.
  Pero no creia en principio que mis habilidades pudieran cubrir las necesidades actuales de los juegos contemporaneos.
  Pensaba que tendría que tener conociminetos profundos en el desarrollo con motores graficos estilo Unity o UnReal, que no tendria la experiencia y capacidad para poder trabajar en ello.
  
  - Pero cuando he visto los ejercicios propuestos en esta empresa vuestra, **BAMTANG**, he visto que era capaz de hacerlos todos ellos, y que mis capacidades actuales podrían superar mis espectativas y podria estar cumpliendo mi sueño.
  Reconozco que no tengo todo el conocimiento para poder resolver todos los problemas.
  Reconozco que las soluciones que podría tener ahora las tengo obsoletas.
  Pero me encanta y me apasionan, quiero aprender, y seguir mejorando.
  
  - Y antes que volver a probar en un nuevo trabajo que no me llene, que no me satisfaga, quiero probar de una vez por todas a trabajar en/con Video Juegos.
  Que como ya he dicho, por ahora, es lo que me motiva y me apasiona.

  - He agregado mi Cv en la carpeta de Curriculum.


**TestBamTang.sln**:  Solucion de Visual Studio 2019. Y estos son las carpetas de los proyectos:
- **BamTang01**  Cypher
  
- **BamTang02**  Laberinto
  
- **BamTang03**  Laberinto 2D
  + Tenemos una 'demo' preparada que se lanza con:    **bamtang03.exe** <*size*> **Demo**   Siendo <size> la dimension del laberinto. Se puede poner por ejemplo: **bamtan03 21 Demo**
  + Los hemos limitado a 25, porque cuando se mostraba en la consola el ocupaba toda la pantalla.
  
- **BamTang05**  Laberinto 3D
  + Al igual que en BamTang03, hemos dejado tambien la misma opcion de 'demo'. Por ejemplo: **bamtan05 21 Demo**
  + Se han quedado con la limitacion de 25.
  + Hemos utilizado la misma carpeta que **'comun/src'** que luego utiliza **'ConsoleApplication1'**.
  
- ***ConsoleApplication1*** para certificar pruebas de concepto.
  + Hemos utilizado la misma carpeta que **'comun/src'** que luego utiliza **'Bamtan05'**.
  + Todo esto era preparatorio para la construcción de la libreria **'suat'**, que utilizaba el mismo codigo.

- **suat**.      Proyecto para la librería dinámica **'suat.dll'**.
  + He agrupado en ella todo el codigo que he ido utilizando en todos los proyectos.
  + Es casi un mini motor de video juegos ya que tiene todo el conocimiento que he ido agrupando a lo largo de los años.
  + Es el mismo codigo que luego esta en la carpeta **'comun/src'**.
  
- **BamTang04**  Fisicas. Hay defectos en las colisiones, pero espero poder aprender de vosotros. Y tengo que dar por terminado el trabajo, porque sino estaría en un proceso de mejora interminable.
  + Ya utiliza la libreria **'suat'**.
 
Insisto, **BamTang05** y **ConsoleApplication1** matienen en comun la carpeta **comun/src**, el mismo código que ya hemos metido en **suat.dll**.
Ha sido un proceso de mejora continua que culmina al final en esta 'dll'.
He puesto el trabajo de Fisicas, **BamTamg04**, al final, ya que ha sido el último que he realizado.
Se supone que todo está para que funciones con el **Visual Studio 2019**.
Y todo esta agrupado en una unica solucion: **'TestBamTang.sln'**.
