param([string]$Compiler = 'C:\msys64\mingw64\bin\g++.exe')
$ErrorActionPreference = 'Stop'
Set-Location -LiteralPath $PSScriptRoot
if (-not (Test-Path -LiteralPath $Compiler)) {
    throw 'No se encontro MinGW-w64. Indica su g++.exe con -Compiler.'
}
$env:PATH = (Split-Path -Parent $Compiler) + ';' + $env:PATH
New-Item -ItemType Directory -Force build | Out-Null
$flags = @('-std=c++17', '-Wall', '-Wextra', '-Werror', '-isystem', 'include',
    '-isystem', 'vendor/box2d/include', '-L.', '-Llib', '-lbox2d', '-lraylib',
    '-lopengl32', '-lgdi32', '-lwinmm', '-static-libgcc', '-static-libstdc++')
& $Compiler main.cpp @flags -o build/Mathmax_Reloaded.exe
if ($LASTEXITCODE -ne 0) { throw 'Fallo la compilacion del juego.' }
& $Compiler tests/test_game.cpp -I. @flags -o build/test_game.exe
if ($LASTEXITCODE -ne 0) { throw 'Fallo la compilacion de las pruebas.' }
& $Compiler main.cpp -DMATHMAX_VERIFY @flags -o build/verify_game.exe
if ($LASTEXITCODE -ne 0) { throw 'Fallo la compilacion de integracion.' }
& ./build/test_game.exe
if ($LASTEXITCODE -ne 0) { throw 'Fallaron las pruebas de regresion.' }

# La prueba grafica usa el archivo de esta copia; se restaura incluso si falla.
$statsPath = Join-Path $PSScriptRoot 'game_stats.dat'
$hadStats = Test-Path -LiteralPath $statsPath
$statsBackup = if ($hadStats) { [System.IO.File]::ReadAllBytes($statsPath) } else { $null }
try {
    & ./build/verify_game.exe
    if ($LASTEXITCODE -ne 0) { throw 'Fallo la prueba de integracion grafica.' }
} finally {
    if ($hadStats) {
        [System.IO.File]::WriteAllBytes($statsPath, $statsBackup)
    } elseif (Test-Path -LiteralPath $statsPath) {
        Remove-Item -LiteralPath $statsPath
    }
}
Write-Output 'COMPILACION Y PRUEBAS COMPLETADAS: salida 0'
