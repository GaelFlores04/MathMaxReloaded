# Actividad 13 Ejecución del plan de pruebas

MATHMAX RELOADED

Ingeniería en Software y Tecnologías Emergentes

Materia: Verificación y Validación del Software

Equipo Loswes

Gael Flores Cañedo | Matrícula 376339

Jorge Roel Vasquez | Matrícula 373850

Docente: Julia Guadalupe Juarez Hernandez

Ensenada, Baja California | 21 de noviembre de 2026

Versión 2.0 | Resultados actualizados

Cierre técnico del alcance A11: 17 casos evaluados, 17 aprobados, 0 rechazados y 0 pendientes. RF02 corregido y comprobado en dos inicios de partida.

## 1 Introducción y objetivo

Esta actividad actualiza la ejecución de CP01-CP17 de A11. Se compara el comportamiento esperado con los resultados obtenidos, se registra la corrección de RF02 y se relacionan los casos con los requisitos y los criterios de aceptación de A12.

## 2 Entorno y procedimiento

La revisión utiliza la adaptación local A10 en Windows x64, C++17, Raylib 5.5 y Box2D 3.1.0. El recorrido funcional emplea entradas programadas, reloj fijo de 1/60 s y estados controlados. En la copia de ensayo se aplaza el disparo enemigo durante conducción y retos para aislar esos resultados. El enemigo inicial se comprueba antes de esa preparación. La entrega conserva los ataques normales. La prueba no se presenta como una nueva sesión con teclado físico.

Se respaldó y restauró game_stats.dat. Su SHA-256 permanece c5bbd40200ffc8f5289bc3fc3937af5c80517fc7f2667e860f8de57b8b0d55d1. Los archivos inválidos se prepararon únicamente en la copia de prueba.

El responsable confirmó la usabilidad con un jugador nuevo y que la sesión de una hora ya fue comprobada. Se conservan esas declaraciones; no se repitió la hora ni se generó un registro técnico ficticio. Las imágenes y los audios se registran como propios por confirmación del responsable.

## 3 Resumen de ejecución

| Indicador | Resultado |
| --- | --- |
| Casos de A11 diseñados y ejecutados | 17 / 17 |
| Aprobados / rechazados / pendientes | 17 / 0 / 0 |
| Recorrido normal | 46 de 46 comprobaciones satisfactorias |
| Recorrido de errores | 47 de 47 comprobaciones satisfactorias |
| Recarga en otro proceso | 3 de 3 comprobaciones satisfactorias |
| Regresión original | 8 grupos aprobados; integración gráfica aprobada |
| Defecto RF02 | Corregido; comprobado en dos inicios de partida |

## CP01 Menú y navegación

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP01 / PA01 / CA01; RF01 RIU01; Exitoso; prioridad alta. |
| Objetivo | Comprobar menú y navegación. |
| Precondiciones | Juego abierto en el menú. |
| Datos de entrada | Flecha abajo, arriba y Enter. |
| Pasos | Recorrer las tres opciones; seleccionar Estadísticas; regresar con ESC. |
| Resultado esperado | Una selección distinguible; Estadísticas abre su pantalla y ESC regresa al menú. |
| Resultado obtenido | Flechas y Enter permiten recorrer las tres opciones, abrir Estadísticas, regresar con ESC y salir normalmente. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | sistema_normal.txt; cuadros 0-9 y 1502-1507 |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP02 Navegación alternativa

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP02 / PA02 / CA02; RF01 RIU01; Alternativo; prioridad alta. |
| Objetivo | Comprobar navegación alternativa. |
| Precondiciones | Menú principal visible. |
| Datos de entrada | S, W y Enter. |
| Pasos | Recorrer opciones con S/W y confirmar Jugar. |
| Resultado esperado | W/S permiten seleccionar las mismas tres opciones que las flechas; Enter inicia. |
| Resultado obtenido | W/S seleccionan las mismas opciones que las flechas; Enter inicia la partida. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | sistema_normal.txt; cuadros 2, 3, 8 y 9 |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP03 Inicio y datos de partida

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP03 / PA03 / CA03; RF02 RIU02; Exitoso; prioridad alta. |
| Objetivo | Comprobar inicio y datos de partida. |
| Precondiciones | Menú y recursos disponibles. |
| Datos de entrada | Jugar y Enter. |
| Pasos | Iniciar una nueva partida; observar vehículo y HUD antes de conducir. |
| Resultado esperado | Vehículo, mundo físico, terreno y enemigos creados; vidas=5, combustible=100 y munición=0 al inicio. |
| Resultado obtenido | Se crean vehículo, terreno y al menos un helicóptero antes del primer cuadro. Los valores iniciales son 5 vidas, combustible 100 y munición 0. Comprobación repetida en dos inicios. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | sistema_normal.txt; RF02 cuadros 11 y 1476; terreno_memoria_unidades.txt |
| Acción y postcondición | RF02: InitializeForRun crea el enemigo inicial; la repetición no duplica enemigos. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP04 Conducción

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP04 / PA04 / CA04; RF03 RIU02; Exitoso; prioridad alta. |
| Objetivo | Comprobar conducción. |
| Precondiciones | Partida nueva con combustible en tramo despejado. |
| Datos de entrada | D durante 2 s; después A durante 2 s. |
| Pasos | Registrar posición; acelerar; frenar o retroceder; comparar desplazamiento e indicadores. |
| Resultado esperado | D produce avance; A reduce velocidad o permite reversa; HUD refleja la partida. |
| Resultado obtenido | Con D, x pasó de 0.015 a 111.072 px y vx a 5.247 m/s. Con A, vx disminuyó a 3.936 m/s y el motor cambió a sentido negativo. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | sistema_normal.txt; medidas D y A, cuadros 139 y 259 |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP05 Sin combustible

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP05 / PA05 / CA05; RF03; Fallo; prioridad alta. |
| Objetivo | Comprobar sin combustible. |
| Precondiciones | Partida con combustible agotado y vehículo estable. |
| Datos de entrada | Combustible=0; D y A. |
| Pasos | Agotar combustible jugando; registrar posición; pulsar D/A y diferenciar inercia o gravedad de fuerza motriz. |
| Resultado esperado | No se aplica fuerza de conducción; no se atribuye movimiento por inercia a un motor activo. |
| Resultado obtenido | Con combustible 0, D y A no activan los motores. La prueba distingue fuerza motriz de inercia. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | sistema_normal.txt; cuadro 1470 |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP06 Disparo y munición

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP06 / PA06 / CA06; RF04 RIU02; Exitoso; prioridad alta. |
| Objetivo | Comprobar disparo y munición. |
| Precondiciones | Partida con al menos un misil recogido. |
| Datos de entrada | Munición=1; Espacio una vez. |
| Pasos | Anotar munición; disparar una vez; comprobar misil y contador. |
| Resultado esperado | Aparece un misil y el contador pasa de 1 a 0 una sola vez. |
| Resultado obtenido | Espacio crea un misil y consume munición de 1 a 0 una sola vez. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | sistema_normal.txt; cuadro 271 |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP07 Disparo sin munición

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP07 / PA07 / CA07; RF04; Fallo; prioridad alta. |
| Objetivo | Comprobar disparo sin munición. |
| Precondiciones | Partida nueva sin misiles. |
| Datos de entrada | Munición=0; Espacio. |
| Pasos | Pulsar Espacio; comprobar contador y ausencia de disparo. |
| Resultado esperado | No se genera misil y munición permanece en 0. |
| Resultado obtenido | Con munición 0, Espacio no crea proyectiles ni vuelve negativo el contador. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | sistema_normal.txt; cuadro 281; P04 |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP08 Escudo y disponibilidad

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP08 / PA08 / CA08; RF05; Exitoso; prioridad alta. |
| Objetivo | Comprobar escudo y disponibilidad. |
| Precondiciones | Partida sin escudo activo ni enfriamiento. |
| Datos de entrada | Shift izquierdo. |
| Pasos | Activar escudo; observar indicador; intentar reactivarlo durante enfriamiento; repetir cuando termine. |
| Resultado esperado | Se activa cuando está disponible; no reinicia por pulsaciones no válidas. Medir por instrumentación la ventana inicial de 0.40 s. |
| Resultado obtenido | Shift activa el escudo durante 3 s y la ventana inicial de parry de 0.40 s. No se reinicia por pulsación repetida ni durante enfriamiento; vuelve a activarse al terminarlo. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | sistema_normal.txt; cuadros 290-600 |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP09 Objetos y límites

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP09 / PA09 / CA09; RF06 RIU02; Alternativo; prioridad alta. |
| Objetivo | Comprobar objetos y límites. |
| Precondiciones | Partida activa y objetos accesibles. |
| Datos de entrada | Combustible cerca de 100; munición cerca de 6. |
| Pasos | Recoger combustible y munición; volver a recoger en el límite; después tocar una mina sin escudo. |
| Resultado esperado | Combustible no supera 100; munición no supera 6; una mina puede producir daño según colisión. |
| Resultado obtenido | Las colisiones recogen combustible y munición sin superar 100 y 6; reparación abre un reto; mina aplica impulso ascendente. El ensayo complementario con control conservado acredita daño indirecto. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | sistema_normal.txt; cuadros 1450-1460; antecedente mina_dano_control.txt |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP10 Reto y recuperación

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP10 / PA10 / CA10; RF07 RIU03; Exitoso; prioridad alta. |
| Objetivo | Comprobar reto y recuperación. |
| Precondiciones | Partida con 4 vidas y reparación recogida. |
| Datos de entrada | Respuesta correcta por clic antes de 10 s. |
| Pasos | Leer operación; calcular respuesta; elegir el rectángulo correcto; observar vidas y cierre del reto. |
| Resultado esperado | Tres opciones distintas, exactamente una correcta; vidas 4 a 5; una sola recompensa y cierre. |
| Resultado obtenido | Se presentan operación, tiempo y tres opciones. El clic en la respuesta correcta recupera de 4 a 5 vidas y cierra el reto sin recompensa duplicada. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | sistema_normal.txt; cuadros 810-811; P01-P02 |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP11 Respuesta incorrecta

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP11 / PA11 / CA11; RF07 RIU03; Fallo; prioridad alta. |
| Objetivo | Comprobar respuesta incorrecta. |
| Precondiciones | Reto activo; registrar vidas antes de responder. |
| Datos de entrada | Una opción incorrecta. |
| Pasos | Resolver operación; seleccionar un distractor; comparar vidas. |
| Resultado esperado | El reto termina sin recuperar vidas. Una entrada no válida debe manejarse sin acceso fuera de rango. |
| Resultado obtenido | El clic en un distractor cierra el reto sin recuperar vidas; índices fuera del arreglo se rechazan en regresión. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | sistema_normal.txt; cuadro 821; P02-P03 |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP12 Tiempo agotado

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP12 / PA12 / CA12; RF07 RIU03; Fallo; prioridad alta. |
| Objetivo | Comprobar tiempo agotado. |
| Precondiciones | Reto activo y vidas registradas. |
| Datos de entrada | No responder durante más de 10 s de simulación. |
| Pasos | Esperar sin pausar; observar cuenta regresiva y cierre. |
| Resultado esperado | Al vencer el plazo el reto termina sin recuperar vidas. |
| Resultado obtenido | El reto sigue activo antes de 10 s y expira después sin recuperar vidas, con tolerancia de un paso de 1/60 s. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | sistema_normal.txt; cuadros 1428 y 1432 |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP13 Pausa y regreso

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP13 / PA13 / CA13; RF08 RIU01; Alternativo; prioridad alta. |
| Objetivo | Comprobar pausa y regreso. |
| Precondiciones | Partida activa. |
| Datos de entrada | ESC; espera 3 s; ESC; ESC y M. |
| Pasos | Pausar; comparar mundo y valores durante espera; reanudar; pausar y volver al menú con M. |
| Resultado esperado | En pausa no avanza simulación; ESC reanuda; M regresa al menú y comunica guardado. |
| Resultado obtenido | Durante 181 pasos en pausa se conservan posición, velocidad, combustible y temporizadores de reto y escudo. ESC reanuda y M vuelve al menú solicitando guardado. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | sistema_normal.txt; cuadros 800, 802 y 1474 |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP14 Fin de partida

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP14 / PA14 / CA14; RF09; Exitoso; prioridad alta. |
| Objetivo | Comprobar fin de partida. |
| Precondiciones | Partida activa; registrar datos de la sesión. |
| Datos de entrada | Daño hasta condición de muerte; M. |
| Pasos | Permitir daño; observar fin de partida; pulsar M; consultar estadísticas. |
| Resultado esperado | Se muestra fin de partida y M lleva al menú; acumulados se guardan una sola vez. |
| Resultado obtenido | La pérdida de la última vida activa fin de partida; M vuelve al menú y solicita guardar. Los acumulados se incorporan una sola vez. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | sistema_normal.txt; cuadros 1480 y 1501 |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP15 Persistencia y récord

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP15 / PA15 / CA15; RF10; Exitoso; prioridad alta. |
| Objetivo | Comprobar persistencia y récord. |
| Precondiciones | Copia aislada del juego con historial respaldado. |
| Datos de entrada | Sesiones de 1000 m/2 bajas/1 parry y 200 m/0/0 para ensayo técnico. |
| Pasos | Guardar; cerrar; abrir; consultar estadísticas; comparar los tres valores; repetir una sesión inferior. |
| Resultado esperado | En ensayo técnico: récord=1000, bajas=2 y parrys=1 tras recarga; récord no disminuye. En interfaz comparar valores observados, sin exigir 1000 m al jugador. |
| Resultado obtenido | Se guardan los datos controlados de 1000 m, 2 bajas y 1 parry. Otro proceso los carga y muestra; una partida inferior no reduce el récord. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | persistencia_nuevo_proceso.txt; P05 |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP16 Archivo legado

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP16 / PA16 / CA16; RF10; Alternativo; prioridad alta. |
| Objetivo | Comprobar archivo legado. |
| Precondiciones | Copia aislada y archivo legado válido respaldado. |
| Datos de entrada | 123.5 m, 7 bajas y 3 parrys; archivo de 12 bytes. |
| Pasos | Cargar legado; comprobar datos; guardar; recargar formato nuevo. |
| Resultado esperado | Conserva los tres valores; migración al guardar de 12 a 24 bytes. |
| Resultado obtenido | La regresión conserva la compatibilidad con el archivo legado de 12 bytes, migra a 24 bytes al guardar y verifica los valores. La recarga ampliada previa conserva 123.5 m, 7 bajas y 3 parrys. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | compilacion_regresion.txt; P06; antecedente regresion_ampliada.txt |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## CP17 Archivo inválido o escritura fallida

| Campo | Registro de ejecución |
| --- | --- |
| Identificación y escenario | CP17 / PA17 / CA17; RF10 RNF04; Fallo; prioridad alta. |
| Objetivo | Comprobar archivo inválido o escritura fallida. |
| Precondiciones | Copia aislada; nunca alterar el historial personal. |
| Datos de entrada | Archivo truncado, NaN, negativos o cabecera inválida; destino de guardado inexistente. |
| Pasos | Intentar cargar cada archivo; comprobar rechazo y conservación de datos válidos; intentar guardar en destino inválido. |
| Resultado esperado | Carga inválida rechazada; datos válidos conservados; guardado devuelve fallo y causa. Mensaje visible al jugador requiere prueba adicional. |
| Resultado obtenido | Se rechazan archivos truncados, NaN, negativos y cabecera inválida sin incorporar valores corruptos. La carga inválida y la escritura bloqueada comunican el fallo sin declarar éxito. |
| Estado | Aprobado en las condiciones de prueba indicadas. |
| Evidencia | sistema_errores.txt; P07-P08; mensaje de carga y guardado |
| Acción y postcondición | Se cumplen las condiciones observadas; conservar registro y repetir el caso si cambia la función. |
| Responsabilidad y fecha | Equipo Loswes. Actualización de resultados del 21 de noviembre de 2026; no se atribuye firma institucional. |

## 4 Trazabilidad y acciones correctivas

| Requisito | Casos de sistema | Resultado |
| --- | --- | --- |
| RF01 | CP01, CP02 | Aprobado |
| RF02 | CP03 | Aprobado |
| RF03 | CP04, CP05 | Aprobado |
| RF04 | CP06, CP07 | Aprobado |
| RF05 | CP08 | Aprobado |
| RF06 | CP09 | Aprobado |
| RF07 | CP10, CP11, CP12 | Aprobado |
| RF08 | CP13 | Aprobado |
| RF09 | CP14 | Aprobado |
| RF10 | CP15, CP16, CP17 | Aprobado |
| RIU01 | CP01, CP02, CP13 | Aprobado |
| RIU02 | CP03, CP04, CP06, CP09 | Aprobado |
| RIU03 | CP10, CP11, CP12 | Aprobado |
| RNF04 | CP17 | Aprobado |

Cobertura por asociación en A11: 14/23 requisitos de A3 (60.9 %). Esta cifra describe la matriz A11; la revisión complementaria de los 23 requisitos se conserva en Cierre_requisitos_Mathmax.md y Matriz_23_requisitos.csv. Las confirmaciones del responsable se identifican por separado.

Evolución del defecto: CP03 pasó de no aprobado por cero enemigos iniciales a aprobado después de incorporar EnemyManager::InitializeForRun y su llamada al crear la partida. El requisito se mantuvo sin alteraciones.

## 5 Evidencias de inicio y pausa

Evidencia gráfica: normal_frame_12.png

normal_frame_12.png

Evidencia gráfica: normal_frame_650.png

normal_frame_650.png

## 6 Evidencias de reto y estadísticas

Evidencia gráfica: normal_frame_809.png

normal_frame_809.png

Evidencia gráfica: estadisticas_real.png

estadisticas_real.png

## 7 Conclusiones y referencias

Los 17 casos del alcance A11 están aprobados en los ensayos definidos. La discrepancia de RF02 quedó corregida y no permanecen casos pendientes en ese alcance. Esta conclusión no equivale a aprobar cualquier escenario posible ni sustituye la decisión institucional del evaluador.

El responsable confirmó la usabilidad con un jugador nuevo y que la sesión de una hora ya fue comprobada. Se conservan esas declaraciones; no se repitió la hora ni se generó un registro técnico ficticio. Las imágenes y los audios se registran como propios por confirmación del responsable.

Repositorio del proyecto: https://github.com/GaelFlores04/MathMaxReloaded

Rama de revisión del código y documentos actualizados: https://github.com/GaelFlores04/MathMaxReloaded/tree/vv/cierre-portafolio-20261121

Fuentes: A3, A11 y A12; Cierre_requisitos_Mathmax.md; registros y capturas de output/Cierre_requisitos; compilacion_regresion.txt; resultados.json y cierre_manifest.json. Los antecedentes de migración y mina se conservan en ejecucion_20261121.

El informe anterior del 16 de noviembre se conserva como antecedente. Esta versión actualiza sus resultados y las evidencias posteriores, sin modificar la fecha de las sesiones anteriores.
