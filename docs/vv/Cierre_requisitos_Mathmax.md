# Cierre de requisitos de Mathmax Reloaded

Fecha de referencia: 21 de noviembre de 2026. Proyecto de Verificacion y Validacion.

Se revisaron los 23 requisitos de A3. El incumplimiento de RF02 se corrigio en el codigo:
ahora se crea un helicoptero antes del primer cuadro de cada partida. La inicializacion
es idempotente y conserva 5 vidas, 100 unidades de combustible y 0 misiles.

Los 17 casos de A11 resultaron satisfactorios en los escenarios definidos. El recorrido
normal obtuvo 46/46 comprobaciones; el de errores, 47/47; la recarga en otro proceso,
3/3. La regresion original aprobo sus ocho grupos y la integracion grafica original.

La usabilidad con jugador nuevo y la sesion de una hora se registran como confirmaciones
del responsable en esta conversacion. No se repitio la hora ni se invento un registro de
ejecucion para ella. El inventario de recursos propios tambien se basa en su declaracion.

## Matriz de los 23 requisitos

| ID | Requisito | Resultado | Evidencia |
|---|---|---|---|
| RF01 | Menu: Jugar, Estadisticas y Salir con W/S, flechas y Enter | Comprobado | CP01-CP02; recorrido normal y recarga |
| RF02 | Partida con vehiculo, terreno, mundo y enemigos; 5 vidas, combustible 100, municion 0 | Corregido y comprobado | CP03 en dos inicios; prueba adicional de inicializacion idempotente |
| RF03 | A/D y motores desactivados sin combustible | Comprobado | CP04-CP05; preparacion controlada sin ataques durante la observacion |
| RF04 | Disparo con consumo unitario y limite de 6 misiles | Comprobado | CP06-CP07 y P04 |
| RF05 | Escudo y ventana de parry de 0.40 s | Comprobado | CP08; activacion, expiracion y enfriamiento |
| RF06 | Recogibles con limites y efecto de minas | Comprobado | CP09; P04 y antecedente de mina con control del 21 de noviembre |
| RF07 | Reto con tres opciones, 10 s y recompensa limitada | Comprobado | CP10-CP12 y P01-P03 |
| RF08 | Pausa sin avance y retorno con M | Comprobado | CP13; posicion, velocidad y temporizadores conservados |
| RF09 | Fin de partida y guardado de acumulados | Comprobado | CP14; retorno con M y acumulacion unica |
| RF10 | Persistencia de distancia, bajas y parrys | Comprobado | CP15-CP16; proceso nuevo y P05-P06 |
| RNF01 | Jugador nuevo completa tareas tras induccion de hasta 10 min | Confirmado por el responsable | El usuario confirmo la prueba en esta conversacion; no se atribuye una nueva sesion observada |
| RNF02 | Objetivo de 60 FPS a 1280 x 720 y reporte de tiempo de cuadro | Medicion documentada | 395 muestras reales; p95 16.9284 ms; ventana oculta; objetivo configurado, sin promesa de 60 FPS sostenidos |
| RNF03 | Sesion de 60 min sin cierre ni crecimiento sostenido de recursos | Confirmado por el responsable | El usuario indico que la hora ya esta comprobada y solicito no repetirla |
| RNF04 | Detectar y comunicar fallos de carga y guardado | Comprobado | CP17; recorrido de errores 47/47 y P07-P08 |
| RIU01 | Seleccion distinguible y ayuda ESC/M en pausa | Comprobado | Revision de pantallas y recorrido de menu/pausa |
| RIU02 | HUD con datos actuales de la partida | Comprobado | Revision de capturas y estados controlados de partida |
| RIU03 | Operacion, tres opciones, temporizador y clic | Comprobado | CP10-CP12; clic correcto e incorrecto y capturas |
| RIE01 | Integracion real de Raylib y Box2D v3 | Comprobado | Compilacion y recorridos con ventana, audio y mundo fisico |
| RIE02 | Rutas relativas y deteccion de recursos faltantes antes del uso | Comprobado | Faltante de textura, faltante de audio y directorio incorrecto: cierre controlado con salida 1 y diagnostico |
| RE01 | Generacion y retiro de terreno; medicion de memoria | Comprobado en recorrido acotado | 1000 segmentos; contadores y memoria real. La hora se conserva por declaracion independiente |
| RDC01 | C++17, Raylib y Box2D compatibles | Comprobado | GCC MinGW-w64, -Wall -Wextra -Werror; binarios Windows x64 |
| RDC02 | 30 pixeles/metro; paso 1/60 s y 20 subpasos | Comprobado | Prueba de constantes y uso de PhysicsWorld::Update |
| RLYE01 | Registro de autores, origen y permisos de recursos y bibliotecas | Documentado | 49 entradas en PROCEDENCIA_RECURSOS.csv; recursos propios segun declaracion del usuario; avisos de terceros conservados |

## Cambios y condiciones de prueba

Se modificaron enemies.h y main.cpp en output/Actividad_10_Mathmax_Reloaded/proyecto/.
EnemyManager::InitializeForRun crea el enemigo inicial si la lista esta vacia; main lo
invoca al crear la partida. La generacion progresiva posterior conserva su comportamiento.
La copia anterior y correccion_RF02.patch permiten revisar el cambio.

El recorrido funcional usa entradas programadas, reloj fijo de 1/60 s y estados controlados.
En esa copia de ensayo se aplaza el disparo del helicoptero durante los casos de conduccion
y reto para que un ataque no invalide la observacion de otra funcion. RF02 se comprueba
antes de esa preparacion. El ejecutable de entrega conserva los ataques normales. Los
resultados no representan una nueva prueba con teclado fisico.

Los ensayos de recursos faltantes son satisfactorios cuando el proceso finaliza con salida
1 y comunica el faltante antes de utilizarlo. Esa salida es intencional, no un caso fallido.
La politica de Control de aplicaciones bloqueo el primer intento; el reintento autorizado
por el usuario permitio completar las ejecuciones posteriores.

## Mediciones adicionales

Equipo: Intel Core i5-1135G7 de 11a generacion; Intel Iris Xe Graphics;
OpenGL 3.3, controlador 30.0.101.1692; Windows x64.

El perfil breve incluye menu, conduccion, pausa y estadisticas, con ventana oculta y
reloj real. Se excluyeron los cinco cuadros iniciales. El percentil 95 fue 16.9284 ms
(aproximadamente 59.1 FPS equivalentes); el maximo aislado fue 184.2639 ms. El objetivo
configurado es 60 FPS. Esto documenta RNF02 y sus limites; no acredita 60 FPS sostenidos
en cualquier escenario ni sustituye la prueba de una hora ya confirmada.

```text
MEASURE RNF02 samples=395 p95_ms=16.9284 min_ms=16.6667 max_ms=184.2639 resolution=1280x720 target_fps=60 window=hidden
PASS RE01 1000 segmentos recorridos; max_bodies=51 max_points=550 memory_warm=8515584 memory_end=8523776 memory_peak=8523776
```

El recorrido espacial usa posiciones controladas sobre 1000 segmentos. Se observaron
550 puntos visuales, 11 segmentos retenidos y hasta 51 cuerpos Box2D tras el calentamiento.
El conjunto de trabajo vario aproximadamente 8 KiB durante ese tramo. Esta medicion es
acotada y no se presenta como una sesion de juego de una hora.

## Procedencia y reproduccion

PROCEDENCIA_RECURSOS.csv enumera 45 recursos de assets/ y cuatro bibliotecas con autor,
origen, permiso y SHA-256. El usuario confirmo que imagenes y audios son del equipo.
No se asigna una licencia publica adicional a esos recursos. Se agregaron CREDITOS.md,
el inventario y LICENSES/ al proyecto. El aviso de GLFW procede de su pagina oficial:
https://www.glfw.org/license. Raylib y Box2D conservan sus avisos locales; winpthreads,
el aviso de la instalacion MinGW-w64. Los binarios de terceros no se modificaron.

Los resultados estan en esta carpeta: sistema_normal.txt, sistema_errores.txt,
persistencia_nuevo_proceso.txt, terreno_memoria_unidades.txt, tiempo_cuadro.txt,
faltante_chasis_truck.txt, faltante_menu_theme.txt, directorio_incorrecto.txt y
compilacion_regresion.txt. resultados.json conserva los codigos de salida.

Las capturas y las fuentes de ensayo se conservan en pruebas/proyecto_instrumentado/.
Las fuentes de entrega estan en ../Actividad_10_Mathmax_Reloaded/proyecto/.
Los scripts tmp/cerrar_requisitos.py y tmp/ejecutar_cierre_requisitos.py preparan y
ejecutan la copia de pruebas. tmp/recompilar_entrega.py recompila la entrega y verifica
su regresion e integracion conservando el historial. Compilador previsto:
C:/msys64/mingw64/bin/g++.exe. Ejecutar los scripts con Python desde la carpeta VV.

Los PDF anteriores conservan los resultados de su fecha. Este cierre reemplaza el
estado tecnico anterior de 16 aprobados y un caso no aprobado; no altera los documentos
historicos ni atribuye firmas o aprobaciones institucionales.
