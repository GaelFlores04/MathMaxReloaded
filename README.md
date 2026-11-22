# Mathmax Reloaded

Videojuego de conducción y acción en C++17 con Raylib 5.5 y Box2D 3.1.0.

## Versión de Verificación y Validación

Esta rama integra la refactorización de A10 y el cierre de requisitos: inicialización
de enemigo desde el primer cuadro (RF02), retos con una sola respuesta correcta,
límites de recursos, acumulación única de estadísticas, persistencia validada y
diagnóstico de recursos faltantes. El código de terceros se conserva sin cambios.

Los 17 casos del alcance A11 pasaron en los escenarios registrados: recorrido normal
46/46, errores 47/47 y recarga en otro proceso 3/3. La regresión original aprobó sus
ocho grupos. Los informes describen datos preparados, condiciones y límites de cada
ensayo. Usabilidad y sesión de una hora constan por confirmación del responsable.

## Compilar y comprobar

Windows x64 y MinGW-w64. Desde la raíz del proyecto:

```powershell
./build.ps1
# Con otro compilador compatible:
./build.ps1 -Compiler 'C:\ruta\bin\g++.exe'
./build/Mathmax_Reloaded.exe
```

Ejecutar desde la raíz para resolver assets/ y game_stats.dat. Se conservan las DLL
recibidas y se agregan los encabezados de Box2D que faltaban. Las bibliotecas recibidas
son de MinGW; no se acredita enlace con MSVC. Las compilaciones usan -Wall -Wextra
-Werror. build.ps1 conserva el historial existente al realizar su integración gráfica.
El ejecutable MatMaxReloaded.exe de la raíz es un antecedente; compilar build/Mathmax_Reloaded.exe
para utilizar los cambios de esta rama.

## Documentación del portafolio

- [A13 Ejecución del plan](docs/vv/VV_A13_Flores_Vasquez_376339_373850.md)
- [A14 Validación del plan](docs/vv/VV_A14_Flores_Vasquez_376339_373850.md)
- [Cierre de los 23 requisitos](docs/vv/Cierre_requisitos_Mathmax.md)
- [Estado del portafolio](docs/vv/Estado_portafolio_Mathmax.md)
- [Matriz de requisitos](docs/vv/Matriz_23_requisitos.csv)

A1 es un caso académico y A6 una auditoría al proyecto Audria. A9 no fue realizada.
Los resultados de otros sistemas se conservan en su contexto y no se atribuyen a Mathmax.
Los informes y el estado de esta rama no representan un portafolio integrado ni una
presentación final ya elaborada.

## Créditos y procedencia

Consultar [CREDITOS.md](CREDITOS.md), [PROCEDENCIA_RECURSOS.csv](PROCEDENCIA_RECURSOS.csv)
y [LICENSES](LICENSES). Los recursos gráficos y sonoros se registran como propios
conforme a la declaración del responsable; no se asigna una licencia pública adicional.
