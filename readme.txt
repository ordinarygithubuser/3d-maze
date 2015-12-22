Namen:
Alexander Clauß & Tilmann Gröger

Buildprozess:
Genutzt wurden GLEW in der Version 1.10 und GLFW in der Version 3.1.2, als Betriebssystem wurde Ubuntu 15.10 genutzt.
Zusätzlich zu den Library-Settings, die in der Manual-Datei beschrieben sind, mussten wir noch folgende drei Libs angeben:
-Xcursor
-dl
-Xinerama
Ansonsten benötigt das Projekt natürlich noch eine Referenz auf das beigelegte scg3-Projekt.

Zur Steuerung gibt es nichts besonderes zu sagen, vorzugsweise ist die Tastatur zu nutzen, da man damit nicht in die Luft steigen kann.
Das Labyrinth kann durch Anpassung der Konstante MAZE_SIZE in der maze.h in der Größe verändert werden. Jedoch sollte mit dieser
Einstellung vorsichtig umgegangen werden, da kein Culling realisiert ist und somit das gesamte Labyrinth immer gerendert wird.