# Actividad 14 Validación del plan de pruebas del sistema

MATHMAX RELOADED

Ingeniería en Software y Tecnologías Emergentes

Materia: Verificación y Validación del Software

Equipo Loswes

Gael Flores Cañedo | Matrícula 376339

Jorge Roel Vasquez | Matrícula 373850

Docente: Julia Guadalupe Juarez Hernandez

Ensenada, Baja California | 21 de noviembre de 2026

Versión 3.0 | Cierre después de corregir RF02

Cierre técnico del alcance A11: 17 casos evaluados, 17 aprobados, 0 rechazados y 0 pendientes. RF02 corregido y comprobado en dos inicios de partida.

## 1 Objetivo y dictamen

El plan A11 permite comprobar los diez requisitos funcionales, tres de interfaz y RNF04 mediante 17 casos. Se validaron diseño, trazabilidad, evidencia y resultados. El dictamen técnico del alcance es satisfactorio: 17 casos ejecutados y aprobados. RF02 fue corregido y comprobado sin cambiar el requisito.

## 2 Criterios de validación

| Criterio | Resultado |
| --- | --- |
| Objetivos y alcance | Identificados: CP01-CP17 y 14 requisitos de A11. |
| Entradas y resultados esperados | Casos con precondiciones, entradas, pasos y postcondiciones verificables. |
| Escenarios | 8 exitosos, 4 alternativos y 5 de fallo en el conjunto. |
| Trazabilidad | Relación requisito-caso conservada; 14/23 requisitos asociados en A11. |
| Evidencia de ejecución | 46/46 comprobaciones normales; 47/47 de errores; recarga 3/3. |
| Acciones correctivas | RF02 corregido; inicialización idempotente y regresión satisfactoria. |
| Aprobación | Dictamen técnico; no se atribuyen firmas ni aprobación institucional. |

## 3 Método

La revisión utiliza la adaptación local A10 en Windows x64, C++17, Raylib 5.5 y Box2D 3.1.0. El recorrido funcional emplea entradas programadas, reloj fijo de 1/60 s y estados controlados. En la copia de ensayo se aplaza el disparo enemigo durante conducción y retos para aislar esos resultados. El enemigo inicial se comprueba antes de esa preparación. La entrega conserva los ataques normales. La prueba no se presenta como una nueva sesión con teclado físico.

## 4 Validación de los casos CP01 a CP09

| Caso | Resultado comprobado | Estado |
| --- | --- | --- |
| CP01 Menú y navegación | Flechas y Enter permiten recorrer las tres opciones, abrir Estadísticas, regresar con ESC y salir normalmente. | Aprobado |
| CP02 Navegación alternativa | W/S seleccionan las mismas opciones que las flechas; Enter inicia la partida. | Aprobado |
| CP03 Inicio y datos de partida | Se crean vehículo, terreno y al menos un helicóptero antes del primer cuadro. Los valores iniciales son 5 vidas, combustible 100 y munición 0. Comprobación repetida en dos inicios. | Aprobado |
| CP04 Conducción | Con D, x pasó de 0.015 a 111.072 px y vx a 5.247 m/s. Con A, vx disminuyó a 3.936 m/s y el motor cambió a sentido negativo. | Aprobado |
| CP05 Sin combustible | Con combustible 0, D y A no activan los motores. La prueba distingue fuerza motriz de inercia. | Aprobado |
| CP06 Disparo y munición | Espacio crea un misil y consume munición de 1 a 0 una sola vez. | Aprobado |
| CP07 Disparo sin munición | Con munición 0, Espacio no crea proyectiles ni vuelve negativo el contador. | Aprobado |
| CP08 Escudo y disponibilidad | Shift activa el escudo durante 3 s y la ventana inicial de parry de 0.40 s. No se reinicia por pulsación repetida ni durante enfriamiento; vuelve a activarse al terminarlo. | Aprobado |
| CP09 Objetos y límites | Las colisiones recogen combustible y munición sin superar 100 y 6; reparación abre un reto; mina aplica impulso ascendente. El ensayo complementario con control conservado acredita daño indirecto. | Aprobado |

## 5 Validación de los casos CP10 a CP17

| Caso | Resultado comprobado | Estado |
| --- | --- | --- |
| CP10 Reto y recuperación | Se presentan operación, tiempo y tres opciones. El clic en la respuesta correcta recupera de 4 a 5 vidas y cierra el reto sin recompensa duplicada. | Aprobado |
| CP11 Respuesta incorrecta | El clic en un distractor cierra el reto sin recuperar vidas; índices fuera del arreglo se rechazan en regresión. | Aprobado |
| CP12 Tiempo agotado | El reto sigue activo antes de 10 s y expira después sin recuperar vidas, con tolerancia de un paso de 1/60 s. | Aprobado |
| CP13 Pausa y regreso | Durante 181 pasos en pausa se conservan posición, velocidad, combustible y temporizadores de reto y escudo. ESC reanuda y M vuelve al menú solicitando guardado. | Aprobado |
| CP14 Fin de partida | La pérdida de la última vida activa fin de partida; M vuelve al menú y solicita guardar. Los acumulados se incorporan una sola vez. | Aprobado |
| CP15 Persistencia y récord | Se guardan los datos controlados de 1000 m, 2 bajas y 1 parry. Otro proceso los carga y muestra; una partida inferior no reduce el récord. | Aprobado |
| CP16 Archivo legado | La regresión conserva la compatibilidad con el archivo legado de 12 bytes, migra a 24 bytes al guardar y verifica los valores. La recarga ampliada previa conserva 123.5 m, 7 bajas y 3 parrys. | Aprobado |
| CP17 Archivo inválido o escritura fallida | Se rechazan archivos truncados, NaN, negativos y cabecera inválida sin incorporar valores corruptos. La carga inválida y la escritura bloqueada comunican el fallo sin declarar éxito. | Aprobado |

## 6 Hallazgos y cierre

| Hallazgo | Acción y evidencia | Estado |
| --- | --- | --- |
| RF02 sin enemigos al inicio | Se crea un helicóptero antes del primer cuadro. RF02 satisfactorio en cuadros 11 y 1476; inicialización repetida no duplica enemigos. | Cerrado |
| Evidencia antes incompleta | A13 actualizado documenta los 17 casos y relaciona registros, capturas y resultados. | Cerrado |
| Errores de carga y guardado | Archivo corrupto y escritura bloqueada generan diagnóstico; se conservan datos válidos. | Comprobado |
| Recursos faltantes | Faltante de textura, faltante de audio y directorio incorrecto cierran con diagnóstico y salida 1 intencional. | Comprobado |
| Límites de alcance | La prueba de una hora y usabilidad se registran por confirmación del responsable. | Registrado |

## 7 Indicadores

Casos diseñados: 17. Ejecutados: 17 (100 %). Aprobados: 17 (100 %). Rechazados: 0. Pendientes: 0. Cobertura por asociación A11: 10/10 RF y 14/23 requisitos de A3. No confundir aprobación de casos con cobertura total del catálogo.

Estado anterior: 16/17 aprobados y CP03 no aprobado por RF02. Estado posterior: 17/17 aprobados tras corregir el inicio. La revisión adicional del catálogo de 23 requisitos se presenta a continuación.

## 8 Revisión complementaria de los 23 requisitos

| ID | Resultado | Evidencia |
| --- | --- | --- |
| RF01 | Comprobado | CP01-CP02; recorrido normal y recarga |
| RF02 | Corregido y comprobado | CP03 en dos inicios; prueba adicional de inicializacion idempotente |
| RF03 | Comprobado | CP04-CP05; preparacion controlada sin ataques durante la observacion |
| RF04 | Comprobado | CP06-CP07 y P04 |
| RF05 | Comprobado | CP08; activacion, expiracion y enfriamiento |
| RF06 | Comprobado | CP09; P04 y antecedente de mina con control del 21 de noviembre |
| RF07 | Comprobado | CP10-CP12 y P01-P03 |
| RF08 | Comprobado | CP13; posicion, velocidad y temporizadores conservados |
| RF09 | Comprobado | CP14; retorno con M y acumulacion unica |
| RF10 | Comprobado | CP15-CP16; proceso nuevo y P05-P06 |
| RNF01 | Confirmado por el responsable | El usuario confirmo la prueba en esta conversacion; no se atribuye una nueva sesion observada |
| RNF02 | Medicion documentada | 395 muestras reales; p95 16.9284 ms; ventana oculta; objetivo configurado, sin promesa de 60 FPS sostenidos |

## 8 Revisión complementaria de los 23 requisitos continuación

| ID | Resultado | Evidencia |
| --- | --- | --- |
| RNF03 | Confirmado por el responsable | El usuario indico que la hora ya esta comprobada y solicito no repetirla |
| RNF04 | Comprobado | CP17; recorrido de errores 47/47 y P07-P08 |
| RIU01 | Comprobado | Revision de pantallas y recorrido de menu/pausa |
| RIU02 | Comprobado | Revision de capturas y estados controlados de partida |
| RIU03 | Comprobado | CP10-CP12; clic correcto e incorrecto y capturas |
| RIE01 | Comprobado | Compilacion y recorridos con ventana, audio y mundo fisico |
| RIE02 | Comprobado | Faltante de textura, faltante de audio y directorio incorrecto: cierre controlado con salida 1 y diagnostico |
| RE01 | Comprobado en recorrido acotado | 1000 segmentos; contadores y memoria real. La hora se conserva por declaracion independiente |
| RDC01 | Comprobado | GCC MinGW-w64, -Wall -Wextra -Werror; binarios Windows x64 |
| RDC02 | Comprobado | Prueba de constantes y uso de PhysicsWorld::Update |
| RLYE01 | Documentado | 49 entradas en PROCEDENCIA_RECURSOS.csv; recursos propios segun declaracion del usuario; avisos de terceros conservados |

## 9 Mediciones y límites

Perfil breve con reloj real: 395 muestras a 1280 x 720, objetivo configurado de 60 FPS, percentil 95 de 16.9284 ms y máximo aislado de 184.2639 ms. Escenario: menú, conducción, pausa y estadísticas con ventana oculta. Equipo: Intel Core i5-1135G7 e Intel Iris Xe; OpenGL 3.3 y controlador 30.0.101.1692. No se afirma 60 FPS sostenidos en cualquier escenario.

Recorrido espacial controlado de 1000 segmentos: tras calentamiento se retienen 11 segmentos y 550 puntos visuales, con hasta 51 cuerpos físicos. Memoria de trabajo aproximadamente 8.12 MiB; variación acotada a unos 8 KiB en ese tramo. Esta prueba no se presenta como una sesión de una hora.

El responsable confirmó la usabilidad con un jugador nuevo y que la sesión de una hora ya fue comprobada. Se conservan esas declaraciones; no se repitió la hora ni se generó un registro técnico ficticio. Las imágenes y los audios se registran como propios por confirmación del responsable.

## 10 Conclusión

El plan y sus resultados son consistentes dentro del alcance A11. La corrección de RF02 permite cerrar la discrepancia funcional antes abierta. El catálogo de A3 distingue comprobaciones técnicas, mediciones y declaraciones del responsable. La exposición y el portafolio deben mantener esa distinción.

## 11 Fuentes y acceso

Repositorio: https://github.com/GaelFlores04/MathMaxReloaded

Rama con fuentes y documentación para revisión: https://github.com/GaelFlores04/MathMaxReloaded/tree/vv/cierre-portafolio-20261121

Referencias: A3, A11, A12 y A13 actualizado; Cierre_requisitos_Mathmax.md; Matriz_23_requisitos.csv; resultados.json; cierre_manifest.json y fuentes_sha256.json. Evidencias en output/Cierre_requisitos. A1 corresponde a un caso académico y A6 a la auditoría del proyecto Audria; sus hallazgos no se atribuyen a Mathmax. A9 no fue realizada y no cuenta con reporte de auditoría.
