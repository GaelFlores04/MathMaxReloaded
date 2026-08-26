#include "fisicaM.h"
#include "carb.h"
#include "enemies.h"
#include "terrain.h"
#include "effects.h"
#include "audios.h"
#include "stats.h"
#include <fstream>

struct ParryLine
{
    Vector2 start;
    Vector2 end;
    float alpha;
};

struct GameTextures
{
    Texture2D carChassis;
    Texture2D carWheel;
    Texture2D helicopter;
    Texture2D missile;
    Texture2D fuelCan;
    Texture2D mine;
    Texture2D ammo;
    Texture2D repair;
    Texture2D menuBackground;
    Texture2D background;
    Texture2D explosionCarTex;
    Texture2D explosionHeliTex;
    Texture2D explosionMissileTex;
    Texture2D explosionMineTex;

    // AGREGAR ESTAS LÍNEAS - Botones
    Texture2D playOff;
    Texture2D playOn;
    Texture2D statsOff;
    Texture2D statsOn;
    Texture2D exitOff;
    Texture2D exitOn;
    Texture2D logo; // Logo para animar
};

enum GameState
{
    MENU,
    PLAYING,
    STATS
};

int main()
{

    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "MATHMAX - RELOADED");
    InitAudioDevice();
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    GameAudio gameAudio;
    gameAudio.Load();

    GameState gameState = MENU;
    GameStats stats;
    stats.Load();
    int selectedOption = 0;
    float logoAnimationTime = 0.0f; // Para animar el logo
    float controlsAlpha = 1.0f;     // Opacidad inicial (visible)
    float controlsTimer = 10.0f;    // Duración en segundos

    // --- CARGAR TEXTURAS ---
    GameTextures tex;
    tex.carChassis = LoadTexture("assets/car/chasis_truck.png");
    tex.carWheel = LoadTexture("assets/car/wheel_truck.png");
    tex.helicopter = LoadTexture("assets/misc/helifull.png");
    tex.missile = LoadTexture("assets/misc/missile.png");
    tex.fuelCan = LoadTexture("assets/misc/fuel.png");
    tex.mine = LoadTexture("assets/misc/mine.png");
    tex.ammo = LoadTexture("assets/misc/ammo.png");
    tex.background = LoadTexture("assets/background/menuB.jpg");
    tex.menuBackground = LoadTexture("assets/background/menuB.jpg"); // AGREGAR ESTA LÍNEA
    tex.repair = LoadTexture("assets/misc/kit.png");
    tex.explosionCarTex = LoadTexture("assets/effects/explosionC.png");
    tex.explosionHeliTex = LoadTexture("assets/effects/explosionH.png");
    tex.explosionMissileTex = LoadTexture("assets/effects/explosionM.png");
    tex.explosionMineTex = LoadTexture("assets/effects/explosionMi.png");

    // AGREGAR ESTAS LÍNEAS - Cargar botones
    tex.playOff = LoadTexture("assets/background/playoff.jpg");
    tex.playOn = LoadTexture("assets/background/playon.jpg");
    tex.statsOff = LoadTexture("assets/background/statsof.jpg");
    tex.statsOn = LoadTexture("assets/background/statson.jpg");
    tex.exitOff = LoadTexture("assets/background/exitoff.jpg");
    tex.exitOn = LoadTexture("assets/background/exiton.jpg");
    tex.logo = LoadTexture("assets/misc/LogoR.png");

    while (true)
    {
        if (IsKeyPressed(KEY_ESCAPE) && gameState == MENU)
            break; // Solo cerrar desde el menú

        if (WindowShouldClose())
            break;
        // ---Audios Menu ---
        if (gameState == MENU || gameState == STATS)
        {
            UpdateMusicStream(gameAudio.menuMusic);
            if (!IsMusicStreamPlaying(gameAudio.menuMusic))
                PlayMusicStream(gameAudio.menuMusic);

            // Sonidos de botones
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_S))
            {
                PlaySound(gameAudio.menuMove);
            }
            if (IsKeyPressed(KEY_ENTER))
            {
                PlaySound(gameAudio.menuSelect);
            }
        }
        else
        {
            StopMusicStream(gameAudio.menuMusic);
        }

        // --- MENÚ PRINCIPAL ---

        if (gameState == MENU)
        {
            logoAnimationTime += GetFrameTime(); // Incrementar tiempo

            BeginDrawing();
            ClearBackground(SKYBLUE);

            // Dibujar fondo escalado
            Rectangle sourceRec = {0, 0, (float)tex.menuBackground.width, (float)tex.menuBackground.height};
            Rectangle destRec = {0, 0, (float)screenWidth, (float)screenHeight};
            DrawTexturePro(tex.menuBackground, sourceRec, destRec, {0, 0}, 0, WHITE);

            // Oscurecer ligeramente
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.15f));

            // --- ANIMAR Y DIBUJAR LOGO ---
            float logoRotation = sin(logoAnimationTime * 2.0f) * 5.0f;
            float logoScale = 1.0f + sin(logoAnimationTime * 3.0f) * 0.05f;

            // Ajustar tamaño del logo si es muy grande
            float logoDisplayWidth = tex.logo.width * logoScale * 0.5f; // Escala al 50%
            float logoDisplayHeight = tex.logo.height * logoScale * 0.5f;
            float logoX = screenWidth / 2.0f - logoDisplayWidth / 2.0f;
            float logoY = -50.0f;

            DrawTextureEx(tex.logo, {logoX, logoY}, logoRotation, logoScale * 0.5f, WHITE);

            // --- DIBUJAR BOTONES CON SEPARACIÓN ---
            int playY = 280;
            int statsY = 400; // +100 de separación
            int exitY = 520;  // +100 de separación
            int buttonWidth = 280;
            int buttonHeight = 80;
            int buttonX = (screenWidth - buttonWidth) / 2;

            // BOTÓN PLAY
            Texture2D playButtonTex = (selectedOption == 0) ? tex.playOn : tex.playOff;
            Rectangle playRect = {(float)buttonX, (float)playY, (float)buttonWidth, (float)buttonHeight};
            DrawTexturePro(playButtonTex,
                           {0, 0, (float)playButtonTex.width, (float)playButtonTex.height},
                           playRect, {0, 0}, 0, WHITE);

            // BOTÓN ESTADÍSTICAS
            Texture2D statsButtonTex = (selectedOption == 1) ? tex.statsOn : tex.statsOff;
            Rectangle statsRect = {(float)buttonX, (float)statsY, (float)buttonWidth, (float)buttonHeight};
            DrawTexturePro(statsButtonTex,
                           {0, 0, (float)statsButtonTex.width, (float)statsButtonTex.height},
                           statsRect, {0, 0}, 0, WHITE);

            // BOTÓN EXIT
            Texture2D exitButtonTex = (selectedOption == 2) ? tex.exitOn : tex.exitOff;
            Rectangle exitRect = {(float)buttonX, (float)exitY, (float)buttonWidth, (float)buttonHeight};
            DrawTexturePro(exitButtonTex,
                           {0, 0, (float)exitButtonTex.width, (float)exitButtonTex.height},
                           exitRect, {0, 0}, 0, WHITE);

            // --- CONTROLES ---
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
                selectedOption = (selectedOption - 1 + 3) % 3;
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
                selectedOption = (selectedOption + 1) % 3;

            if (IsKeyPressed(KEY_ENTER))
            {
                if (selectedOption == 0)
                    gameState = PLAYING;
                else if (selectedOption == 1)
                    gameState = STATS;
                else if (selectedOption == 2)
                    break;
            }

            EndDrawing();
        }

        // --- PANTALLA DE ESTADÍSTICAS ---
        // --- PANTALLA DE ESTADÍSTICAS ---
        else if (gameState == STATS)
        {
            BeginDrawing();
            ClearBackground(SKYBLUE);

            // Dibujar fondo escalado igual al menú
            Rectangle sourceRec = {0, 0, (float)tex.menuBackground.width, (float)tex.menuBackground.height};
            Rectangle destRec = {0, 0, (float)screenWidth, (float)screenHeight};
            DrawTexturePro(tex.menuBackground, sourceRec, destRec, {0, 0}, 0, WHITE);

            // Oscurecer ligeramente
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.25f));

            // Panel principal estilo terminal
            int panelX = 150;
            int panelY = 80;
            int panelWidth = screenWidth - 300;
            int panelHeight = screenHeight - 160;

            // Borde exterior grueso (estilo terminal)
            DrawRectangle(panelX - 4, panelY - 4, panelWidth + 8, panelHeight + 8, {20, 200, 255, 255});
            DrawRectangle(panelX - 2, panelY - 2, panelWidth + 4, panelHeight + 4, {10, 100, 150, 255});
            DrawRectangle(panelX, panelY, panelWidth, panelHeight, {30, 50, 80, 255});

            // Líneas decorativas estilo terminal (CORREGIDO)
            DrawLineEx({(float)(panelX + 10), (float)(panelY + 50)}, {(float)(panelX + panelWidth - 10), (float)(panelY + 50)}, 2, {20, 200, 255, 150});
            DrawLineEx({(float)(panelX + 10), (float)(panelY + panelHeight - 50)}, {(float)(panelX + panelWidth - 10), (float)(panelY + panelHeight - 50)}, 2, {20, 200, 255, 150});

            // Título con estilo
            const char *titleMain = "[ ESTADISTICAS DE JUEGO ]";
            int titleWidth = MeasureText(titleMain, 40);
            DrawText(titleMain, screenWidth / 2 - titleWidth / 2, panelY + 15, 40, {20, 255, 150, 255});

            // Línea separadora (CORREGIDO)
            DrawLineEx({(float)(panelX + 20), (float)(panelY + 65)}, {(float)(panelX + panelWidth - 20), (float)(panelY + 65)}, 1, {20, 200, 255, 100});

            // ESTADÍSTICAS CON MARCOS
            int statY = panelY + 100;
            int statSpacing = 100;
            int statBoxHeight = 70;
            int statBoxWidth = panelWidth - 40;
            int statBoxX = panelX + 20;

            // ===== ESTADÍSTICA 1: Distancia =====
            DrawRectangleLines(statBoxX, statY, statBoxWidth, statBoxHeight, {20, 200, 255, 255});
            DrawRectangle(statBoxX + 2, statY + 2, statBoxWidth - 4, statBoxHeight - 4, Fade({20, 100, 150, 255}, 0.3f));

            DrawText(">>> DISTANCIA MAXIMA <<<", statBoxX + 20, statY + 10, 20, {20, 255, 150, 255});
            DrawText(TextFormat("%.0f METROS", stats.maxDistance), statBoxX + 20, statY + 35, 24, {255, 255, 100, 255});

            // ===== ESTADÍSTICA 2: Helicópteros =====
            statY += statSpacing;
            DrawRectangleLines(statBoxX, statY, statBoxWidth, statBoxHeight, {255, 100, 50, 255});
            DrawRectangle(statBoxX + 2, statY + 2, statBoxWidth - 4, statBoxHeight - 4, Fade({200, 50, 50, 255}, 0.3f));

            DrawText(">>> HELICOPTEROS DERRIBADOS <<<", statBoxX + 20, statY + 10, 20, {255, 100, 50, 255});
            DrawText(TextFormat("%d BAJAS", stats.helicoptersDestroyed), statBoxX + 20, statY + 35, 24, {255, 150, 100, 255});

            // ===== ESTADÍSTICA 3: Parrys =====
            statY += statSpacing;
            DrawRectangleLines(statBoxX, statY, statBoxWidth, statBoxHeight, {150, 50, 255, 255});
            DrawRectangle(statBoxX + 2, statY + 2, statBoxWidth - 4, statBoxHeight - 4, Fade({150, 50, 255, 255}, 0.2f));

            DrawText(">>> PARRYS REALIZADOS <<<", statBoxX + 20, statY + 10, 20, {150, 100, 255, 255});
            DrawText(TextFormat("%d DEFLEXIONES", stats.parrysPerformed), statBoxX + 20, statY + 35, 24, {200, 150, 255, 255});

            // Línea separadora inferior (CORREGIDO)
            DrawLineEx({(float)(panelX + 20), (float)(panelY + panelHeight - 55)}, {(float)(panelX + panelWidth - 20), (float)(panelY + panelHeight - 55)}, 1, {20, 200, 255, 100});

            // Instrucciones con estilo
            const char *backMsg = "[ PRESIONA ESC PARA VOLVER AL MENU ]";
            int backWidth = MeasureText(backMsg, 18);
            DrawText(backMsg, screenWidth / 2 - backWidth / 2, panelY + panelHeight - 35, 18, {20, 255, 200, 255});

            // Decoración: pequeños cuadros en las esquinas
            DrawRectangle(panelX + 5, panelY + 5, 15, 15, {20, 200, 255, 150});
            DrawRectangle(panelX + panelWidth - 20, panelY + 5, 15, 15, {20, 200, 255, 150});
            DrawRectangle(panelX + 5, panelY + panelHeight - 20, 15, 15, {20, 200, 255, 150});
            DrawRectangle(panelX + panelWidth - 20, panelY + panelHeight - 20, 15, 15, {20, 200, 255, 150});

            if (IsKeyPressed(KEY_ESCAPE))
            {
                gameState = MENU;
                selectedOption = 0;
            }

            EndDrawing();
        }

        // --- JUEGO ---
        // --- JUEGO ---
        else if (gameState == PLAYING)
        {
            PhysicsWorld physics;
            Car myCar(physics.GetWorldId(), 0, 300);
            Terrain terrain(physics.GetWorldId(), 30.0f);
            EnemyManager enemies(physics.GetWorldId(), 30.0f, tex.helicopter, tex.missile);
            ExplosionManager explosionManager;

            myCar.SetExplosionManager(&explosionManager);
            myCar.SetExplosionTextures(tex.explosionCarTex, tex.explosionMissileTex);

            enemies.SetExplosionManager(&explosionManager);
            enemies.SetExplosionTexture(tex.explosionHeliTex);
            enemies.SetMissileExplosionTexture(tex.explosionMissileTex);

            terrain.SetExplosionManager(&explosionManager);
            terrain.SetExplosionTexture(tex.explosionMineTex);

            myCar.SetAudio(&gameAudio);
            enemies.SetAudio(&gameAudio);
            terrain.SetAudio(&gameAudio);

            StopMusicStream(gameAudio.menuMusic); // <--- AGREGAR
            PlayMusicStream(gameAudio.gameMusic);

            Camera2D camera = {0};
            camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
            camera.zoom = 1.0f;

            int lastEnemyCount = 0;
            float enemyAlertTimer = 0.0f;
            float flashTimer = 0.0f;
            float hitStopTimer = 0.0f;
            std::vector<ParryLine> parryLines;
            float maxDistanceThisRun = 0.0f;
            int parrysThisRun = 0;
            int helicoptersThisRun = 0;
            bool paused = false;
            bool escPressed = false;
            bool alreadyDestroyed = false;

            while (!WindowShouldClose() && gameState == PLAYING)
            {
                UpdateMusicStream(gameAudio.gameMusic); // <--- OBLIGATORIO AQUÍ
                float dt = GetFrameTime();

                // Dentro del bucle while, en la sección de UPDATE
                if (controlsTimer > 0.0f)
                {
                    controlsTimer -= dt;
                    // Cuando queden menos de 2 segundos, empezamos a bajar la opacidad
                    if (controlsTimer < 2.0f)
                    {
                        controlsAlpha = controlsTimer / 2.0f;
                    }
                }

                explosionManager.Update(dt);

                if (IsKeyPressed(KEY_ESCAPE) && !escPressed)
                {
                    paused = !paused;
                    escPressed = true;

                    if (paused)
                    {
                        PauseSound(gameAudio.engine);
                        PauseSound(gameAudio.heliRotor);
                        // La música del juego sigue sonando (no la pausamos)
                    }
                    else
                    {
                        ResumeSound(gameAudio.engine);
                        ResumeSound(gameAudio.heliRotor);
                    }
                }

                if (!IsKeyDown(KEY_ESCAPE))
                {
                    escPressed = false;
                }

                if (paused)
                {
                    BeginDrawing();

                    // 1. CIELO (Gradiente Naranja - Atardecer)
                    // Dibuja un rectángulo vertical degradado de naranja oscuro arriba a amarillo abajo
                    DrawRectangleGradientV(0, 0, screenWidth, screenHeight,
                                           (Color){255, 140, 0, 255},    // Naranja oscuro (Top)
                                           (Color){255, 200, 100, 255}); // Amarillo suave (Bottom)

                    // 2. SOL (Círculo estático)
                    DrawCircle(screenWidth - 150, 150, 60, (Color){255, 255, 0, 200});   // Centro
                    DrawCircle(screenWidth - 150, 150, 70, (Color){255, 255, 200, 100}); // Brillo externo

                    // --- LÓGICA PARALLAX ---
                    // Usamos la posición X del coche para calcular el desplazamiento
                    float carX = myCar.GetPosition().x;
                    float scaleFactor = 1.0f; // Ajusta si tus edificios se ven muy grandes/pequeños

                    // ---------------------------------------------------------
                    // CAPA 1: EDIFICIOS LEJANOS (Se mueven muy lento: 5% velocidad)
                    // Color oscuro y siluetas simples para dar profundidad
                    // ---------------------------------------------------------
                    float farSpeed = 0.05f;
                    float farOffset = -(carX * farSpeed);
                    Color farColor = (Color){80, 40, 40, 255}; // Marrón muy oscuro/rojizo

                    // Simulamos edificios infinitos usando módulo
                    // Un "bloque" de edificios lejanos mide, digamos, 2000 pixeles
                    float farPatternWidth = 2000.0f;
                    float currentFarX = fmod(farOffset, farPatternWidth);

                    // Dibujamos el patrón dos veces para cubrir la pantalla al hacer scroll
                    for (int j = 0; j < 2; j++)
                    {
                        float baseX = currentFarX + (j * farPatternWidth);

                        // Semilla fija para que los edificios siempre sean iguales en el mismo lugar
                        // (Usamos un truco simple: dibujamos rectángulos fijos relativos al baseX)

                        // Dibujamos 20 edificios por bloque
                        for (int i = 0; i < 20; i++)
                        {
                            // Generación pseudo-aleatoria basada en el índice 'i'
                            float h = 100.0f + (sinf(i * 132.0f) * 50.0f) + 150.0f; // Altura variable
                            float w = 80.0f + (cosf(i * 45.0f) * 30.0f);            // Ancho variable
                            float x = baseX + (i * 100.0f);                         // Posición

                            if (x + w > 0 && x < screenWidth)
                            { // Solo dibujar si está en pantalla
                                DrawRectangle(x, screenHeight - h - 50, w, h + 100, farColor);
                                // Ventanas lejanas (puntos pequeños)
                                if (i % 3 == 0)
                                    DrawRectangle(x + 10, screenHeight - h + 20, 10, 10, (Color){255, 200, 100, 100});
                            }
                        }
                    }

                    // ---------------------------------------------------------
                    // CAPA 2: EDIFICIOS MEDIOS (Se mueven más rápido: 20% velocidad)
                    // Un poco más claros y detallados
                    // ---------------------------------------------------------
                    float midSpeed = 0.2f;
                    float midOffset = -(carX * midSpeed);
                    Color midColor = (Color){60, 30, 30, 255}; // Un poco más oscuro que el fondo lejano

                    float midPatternWidth = 1500.0f;
                    float currentMidX = fmod(midOffset, midPatternWidth);

                    for (int j = 0; j < 2; j++)
                    { // Dibujar 2 copias para el loop
                        float baseX = currentMidX + (j * midPatternWidth);

                        // Dibujamos 15 edificios por bloque
                        for (int i = 0; i < 15; i++)
                        {
                            float h = 150.0f + (cosf(i * 54.0f) * 80.0f) + 100.0f;
                            float w = 90.0f + (sinf(i * 22.0f) * 40.0f);
                            float x = baseX + (i * 120.0f);

                            if (x + w > 0 && x < screenWidth)
                            {
                                // Cuerpo edificio
                                DrawRectangle(x, screenHeight - h - 20, w, h + 100, midColor);

                                // Detalle: Borde iluminado por el atardecer (lado izquierdo)
                                DrawRectangle(x, screenHeight - h - 20, 5, h + 100, (Color){255, 100, 50, 50});

                                // Detalle: Ventanas iluminadas aleatorias
                                for (int wy = 0; wy < (int)(h / 30); wy++)
                                {
                                    for (int wx = 0; wx < (int)(w / 25); wx++)
                                    {
                                        // "Ruido" determinista para encender algunas luces
                                        if (((i + wx + wy) % 5) == 0)
                                        {
                                            DrawRectangle(x + 10 + (wx * 20), screenHeight - h + 20 + (wy * 30), 12, 18, (Color){255, 255, 150, 150});
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // --- AQUI EMPIEZA TU MODO 2D (JUEGO REAL) ---
                    BeginMode2D(camera);

                    terrain.Draw(tex.fuelCan, tex.mine, tex.ammo, tex.repair);
                    enemies.Draw();
                    myCar.Draw(tex.carChassis, tex.carWheel, tex.missile);
                    explosionManager.Draw();

                    EndMode2D();

                    // Panel de pausa semitransparente
                    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));

                    // Panel principal
                    int pausePanelX = screenWidth / 2 - 300;
                    int pausePanelY = screenHeight / 2 - 180;
                    int pausePanelW = 600;
                    int pausePanelH = 360;

                    // Borde exterior
                    DrawRectangle(pausePanelX - 4, pausePanelY - 4, pausePanelW + 8, pausePanelH + 8, {20, 200, 255, 255});
                    DrawRectangle(pausePanelX - 2, pausePanelY - 2, pausePanelW + 4, pausePanelH + 4, {10, 100, 150, 255});
                    DrawRectangle(pausePanelX, pausePanelY, pausePanelW, pausePanelH, {30, 50, 80, 255});

                    // Título PAUSA
                    const char *pauseTitle = "[ PAUSA ]";
                    int pauseTitleWidth = MeasureText(pauseTitle, 60);
                    DrawText(pauseTitle, screenWidth / 2 - pauseTitleWidth / 2, pausePanelY + 20, 60, {20, 255, 150, 255});

                    // Línea separadora
                    DrawLineEx({(float)(pausePanelX + 20), (float)(pausePanelY + 90)}, {(float)(pausePanelX + pausePanelW - 20), (float)(pausePanelY + 90)}, 2, {20, 200, 255, 150});

                    // Opciones
                    int optionY = pausePanelY + 130;
                    int optionSpacing = 80;

                    // Opción 1: Reanudar
                    DrawRectangleLines(pausePanelX + 30, optionY, pausePanelW - 60, 60, {20, 255, 150, 255});
                    DrawRectangle(pausePanelX + 32, optionY + 2, pausePanelW - 64, 56, Fade({20, 150, 100, 255}, 0.3f));
                    DrawText("[ ESC ] REANUDAR JUEGO", pausePanelX + 50, optionY + 15, 20, {20, 255, 150, 255});

                    // Opción 2: Volver al menú
                    optionY += optionSpacing;
                    DrawRectangleLines(pausePanelX + 30, optionY, pausePanelW - 60, 60, {255, 100, 50, 255});
                    DrawRectangle(pausePanelX + 32, optionY + 2, pausePanelW - 64, 56, Fade({200, 50, 50, 255}, 0.3f));
                    DrawText("[ M ] VOLVER AL MENU", pausePanelX + 50, optionY + 15, 20, {255, 100, 50, 255});

                    // Cuadros decorativos en esquinas
                    DrawRectangle(pausePanelX + 10, pausePanelY + 10, 15, 15, {20, 200, 255, 150});
                    DrawRectangle(pausePanelX + pausePanelW - 25, pausePanelY + 10, 15, 15, {20, 200, 255, 150});
                    DrawRectangle(pausePanelX + 10, pausePanelY + pausePanelH - 25, 15, 15, {20, 200, 255, 150});
                    DrawRectangle(pausePanelX + pausePanelW - 25, pausePanelY + pausePanelH - 25, 15, 15, {20, 200, 255, 150});

                    if (IsKeyPressed(KEY_M))
                    {
                        stats.UpdateMaxDistance(maxDistanceThisRun);
                        stats.helicoptersDestroyed += enemies.GetActiveHelicopterCount();
                        stats.parrysPerformed += parrysThisRun;
                        stats.Save();

                        StopSound(gameAudio.engine);          // <--- AÑADIR
                        StopSound(gameAudio.heliRotor);       // <--- AÑADIR
                        StopMusicStream(gameAudio.gameMusic); // <--- AÑADIR

                        gameState = MENU;
                        selectedOption = 0;
                        paused = false;
                        escPressed = false;

                        break;
                    }

                    EndDrawing();
                    continue;
                }

                // --- HIT STOP ---
                if (hitStopTimer > 0.0f)
                {
                    hitStopTimer -= dt;
                    BeginDrawing();
                    BeginMode2D(camera);

                    terrain.Draw(tex.fuelCan, tex.mine, tex.ammo, tex.repair);
                    myCar.Draw(tex.carChassis, tex.carWheel, tex.missile);
                    enemies.Draw();
                    explosionManager.Draw();
                    for (const auto &line : parryLines)
                        DrawLineEx(line.start, line.end, 10.0f, Fade(WHITE, line.alpha));
                    EndMode2D();
                    if (flashTimer > 0.0f)
                        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(WHITE, flashTimer * 2.0f));
                    EndDrawing();
                    continue;
                }

                if (flashTimer > 0.0f)
                    flashTimer -= dt * 5.0f;
                for (auto it = parryLines.begin(); it != parryLines.end();)
                {
                    it->alpha -= dt * 2.0f;
                    if (it->alpha <= 0.0f)
                        it = parryLines.erase(it);
                    else
                        ++it;
                }

                myCar.UpdateInput();
                myCar.UpdateInvulnerability(dt);
                myCar.UpdateMissiles([&](float x)
                                     { return terrain.GetHeightAt(x); });

                physics.Update();
                Vector2 carPos = myCar.GetPosition();
                maxDistanceThisRun = carPos.x / 30.0f;

                maxDistanceThisRun = carPos.x / 30.0f;

                // 1. Calculamos la altura del suelo aquí
                float groundHeight = terrain.GetHeightAt(carPos.x);

                // 2. Le pasamos la altura a la función para que ella se encargue
                myCar.CheckHeadCrash(groundHeight);

                if (myCar.IsDead() && !myCar.IsQuizActive())
                {
                    if (!alreadyDestroyed)
                    {
                        // NO BORRAMOS EL CUERPO, solo lo "apagamos"
                        myCar.StopMovement(); // <--- LLAMAR AL NUEVO MÉTODO

                        alreadyDestroyed = true;
                        explosionManager.AddCarExplosion(myCar.GetPosition(), tex.explosionCarTex);
                    }
                }

                terrain.Update(carPos, myCar);
                if (!myCar.IsDead())
                {
                    enemies.Update(myCar, dt);

                    Vector2 parryTargetPos;
                    bool didParry = enemies.UpdateEnemyMissiles(myCar, parryTargetPos);
                    if (didParry)
                    {
                        parrysThisRun++;
                        hitStopTimer = 0.75f;
                        flashTimer = 0.45f;
                        parryLines.push_back({carPos, parryTargetPos, 1.0f});
                    }
                }

                // --- MÚSICA DE JUEGO ---
                if (gameState == PLAYING)
                {
                    UpdateMusicStream(gameAudio.gameMusic);
                    if (!IsMusicStreamPlaying(gameAudio.gameMusic))
                        PlayMusicStream(gameAudio.gameMusic);

                    // Loop del helicóptero (si hay enemigos vivos)
                    if (enemies.GetActiveHelicopterCount() > 0)
                    {
                        if (!IsSoundPlaying(gameAudio.heliRotor))
                            PlaySound(gameAudio.heliRotor);
                    }
                    else
                    {
                        StopSound(gameAudio.heliRotor);
                    }
                }

                Vector2 parryTargetPos;
                bool didParry = enemies.UpdateEnemyMissiles(myCar, parryTargetPos);
                if (didParry)
                {
                    parrysThisRun++;
                    hitStopTimer = 0.75f;
                    flashTimer = 0.45f;
                    parryLines.push_back({carPos, parryTargetPos, 1.0f});
                }

                int currentCount = enemies.GetActiveHelicopterCount();

                if (enemies.GetActiveHelicopterCount() > 0)
                {
                    if (!IsSoundPlaying(gameAudio.heliRotor))
                        PlaySound(gameAudio.heliRotor);
                }
                else
                {
                    StopSound(gameAudio.heliRotor);
                }

                if (currentCount > lastEnemyCount)
                    enemyAlertTimer = 2.5f;
                lastEnemyCount = currentCount;
                if (enemyAlertTimer > 0.0f)
                {
                    enemyAlertTimer -= dt;
                    if (enemyAlertTimer < 0.0f)
                        enemyAlertTimer = 0.0f;
                }

                float groundY = 650.0f;
                float carY = carPos.y;
                float heightDiff = groundY - carY;
                Vector2 targetPos = carPos;
                targetPos.x += 200.0f;
                if (heightDiff > 0)
                    targetPos.y = carY + (heightDiff * 0.3f);
                camera.target = targetPos;

                float baseZoom = 0.70f;
                float targetZoom = baseZoom;
                if (heightDiff > 100.0f)
                {
                    float zoomFactor = (heightDiff - 100.0f) / 1400.0f;
                    targetZoom = baseZoom - (zoomFactor * 0.4f);
                }
                if (targetZoom < 0.3f)
                    targetZoom = 0.3f;
                if (targetZoom > baseZoom)
                    targetZoom = baseZoom;
                camera.zoom += (targetZoom - camera.zoom) * 0.05f;
                camera.offset = (Vector2){(float)screenWidth / 2.0f, (float)screenHeight / 2.0f};

                // --- CHEQUEAR SI ESTÁ MUERTO ---
                if (myCar.IsDead())
                {
                    static bool alreadyExploded = false;

                    explosionManager.Update(dt);

                    if (!alreadyDestroyed) // Variable que ya tienes definida
                    {
                        // 1. Obtener posición ANTES de destruir
                        Vector2 deathPos = myCar.GetPosition();

                        // 2. Crear explosión
                        explosionManager.AddCarExplosion(deathPos, tex.explosionCarTex);

                        // 3. Destruir física
                        b2DestroyBody(myCar.GetBodyId());
                        b2DestroyBody(myCar.GetRoofSensorId());

                        alreadyDestroyed = true;
                    }

                    BeginDrawing();

                    // 1. CIELO (Gradiente Naranja - Atardecer)
                    // Dibuja un rectángulo vertical degradado de naranja oscuro arriba a amarillo abajo
                    DrawRectangleGradientV(0, 0, screenWidth, screenHeight,
                                           (Color){255, 140, 0, 255},    // Naranja oscuro (Top)
                                           (Color){255, 200, 100, 255}); // Amarillo suave (Bottom)

                    // 2. SOL (Círculo estático)
                    DrawCircle(screenWidth - 150, 150, 60, (Color){255, 255, 0, 200});   // Centro
                    DrawCircle(screenWidth - 150, 150, 70, (Color){255, 255, 200, 100}); // Brillo externo

                    // --- LÓGICA PARALLAX ---
                    // Usamos la posición X del coche para calcular el desplazamiento
                    float carX = myCar.GetPosition().x;
                    float scaleFactor = 1.0f; // Ajusta si tus edificios se ven muy grandes/pequeños

                    // ---------------------------------------------------------
                    // CAPA 1: EDIFICIOS LEJANOS (Se mueven muy lento: 5% velocidad)
                    // Color oscuro y siluetas simples para dar profundidad
                    // ---------------------------------------------------------
                    float farSpeed = 0.05f;
                    float farOffset = -(carX * farSpeed);
                    Color farColor = (Color){80, 40, 40, 255}; // Marrón muy oscuro/rojizo

                    // Simulamos edificios infinitos usando módulo
                    // Un "bloque" de edificios lejanos mide, digamos, 2000 pixeles
                    float farPatternWidth = 2000.0f;
                    float currentFarX = fmod(farOffset, farPatternWidth);

                    // Dibujamos el patrón dos veces para cubrir la pantalla al hacer scroll
                    for (int j = 0; j < 2; j++)
                    {
                        float baseX = currentFarX + (j * farPatternWidth);

                        // Semilla fija para que los edificios siempre sean iguales en el mismo lugar
                        // (Usamos un truco simple: dibujamos rectángulos fijos relativos al baseX)

                        // Dibujamos 20 edificios por bloque
                        for (int i = 0; i < 20; i++)
                        {
                            // Generación pseudo-aleatoria basada en el índice 'i'
                            float h = 100.0f + (sinf(i * 132.0f) * 50.0f) + 150.0f; // Altura variable
                            float w = 80.0f + (cosf(i * 45.0f) * 30.0f);            // Ancho variable
                            float x = baseX + (i * 100.0f);                         // Posición

                            if (x + w > 0 && x < screenWidth)
                            { // Solo dibujar si está en pantalla
                                DrawRectangle(x, screenHeight - h - 50, w, h + 100, farColor);
                                // Ventanas lejanas (puntos pequeños)
                                if (i % 3 == 0)
                                    DrawRectangle(x + 10, screenHeight - h + 20, 10, 10, (Color){255, 200, 100, 100});
                            }
                        }
                    }

                    // ---------------------------------------------------------
                    // CAPA 2: EDIFICIOS MEDIOS (Se mueven más rápido: 20% velocidad)
                    // Un poco más claros y detallados
                    // ---------------------------------------------------------
                    float midSpeed = 0.2f;
                    float midOffset = -(carX * midSpeed);
                    Color midColor = (Color){60, 30, 30, 255}; // Un poco más oscuro que el fondo lejano

                    float midPatternWidth = 1500.0f;
                    float currentMidX = fmod(midOffset, midPatternWidth);

                    for (int j = 0; j < 2; j++)
                    { // Dibujar 2 copias para el loop
                        float baseX = currentMidX + (j * midPatternWidth);

                        // Dibujamos 15 edificios por bloque
                        for (int i = 0; i < 15; i++)
                        {
                            float h = 150.0f + (cosf(i * 54.0f) * 80.0f) + 100.0f;
                            float w = 90.0f + (sinf(i * 22.0f) * 40.0f);
                            float x = baseX + (i * 120.0f);

                            if (x + w > 0 && x < screenWidth)
                            {
                                // Cuerpo edificio
                                DrawRectangle(x, screenHeight - h - 20, w, h + 100, midColor);

                                // Detalle: Borde iluminado por el atardecer (lado izquierdo)
                                DrawRectangle(x, screenHeight - h - 20, 5, h + 100, (Color){255, 100, 50, 50});

                                // Detalle: Ventanas iluminadas aleatorias
                                for (int wy = 0; wy < (int)(h / 30); wy++)
                                {
                                    for (int wx = 0; wx < (int)(w / 25); wx++)
                                    {
                                        // "Ruido" determinista para encender algunas luces
                                        if (((i + wx + wy) % 5) == 0)
                                        {
                                            DrawRectangle(x + 10 + (wx * 20), screenHeight - h + 20 + (wy * 30), 12, 18, (Color){255, 255, 150, 150});
                                        }
                                    }
                                }
                            }
                        }
                    }

                    BeginMode2D(camera);

                    terrain.Draw(tex.fuelCan, tex.mine, tex.ammo, tex.repair);
                    enemies.Draw();
                    explosionManager.Draw();

                    EndMode2D();

                    // Panel de game over semitransparente
                    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.8f));

                    // Panel principal GAME OVER
                    int goX = screenWidth / 2 - 350;
                    int goY = screenHeight / 2 - 200;
                    int goW = 700;
                    int goH = 400;

                    // Borde exterior
                    DrawRectangle(goX - 4, goY - 4, goW + 8, goH + 8, {255, 50, 50, 255});
                    DrawRectangle(goX - 2, goY - 2, goW + 4, goH + 4, {150, 20, 20, 255});
                    DrawRectangle(goX, goY, goW, goH, {60, 20, 20, 255});

                    // Título GAME OVER
                    const char *gameOverTitle = "[ GAME OVER ]";
                    int goTitleWidth = MeasureText(gameOverTitle, 80);
                    DrawText(gameOverTitle, screenWidth / 2 - goTitleWidth / 2, goY + 20, 80, {255, 100, 50, 255});

                    // Línea separadora
                    DrawLineEx({(float)(goX + 20), (float)(goY + 120)}, {(float)(goX + goW - 20), (float)(goY + 120)}, 2, {255, 100, 50, 150});

                    // Estadísticas de la partida
                    int statY = goY + 150;
                    DrawText(TextFormat("DISTANCIA: %.0f m", maxDistanceThisRun), goX + 40, statY, 24, {20, 255, 150, 255});
                    statY += 50;
                    DrawText(TextFormat("PARRYS: %d", parrysThisRun), goX + 40, statY, 24, {150, 100, 255, 255});
                    statY += 50;
                    DrawText(TextFormat("HELICOPTEROS: %d", enemies.GetHelicoptersDestroyedCount()), goX + 40, statY, 24, {255, 150, 100, 255});

                    // Línea separadora inferior
                    DrawLineEx({(float)(goX + 20), (float)(goY + goH - 80)}, {(float)(goX + goW - 20), (float)(goY + goH - 80)}, 2, {255, 100, 50, 150});

                    // Instrucción
                    const char *goMsg = "[ PRESIONA M PARA VOLVER AL MENU ]";
                    int goMsgWidth = MeasureText(goMsg, 20);
                    DrawText(goMsg, screenWidth / 2 - goMsgWidth / 2, goY + goH - 50, 20, {20, 255, 200, 255});

                    // Cuadros decorativos en esquinas
                    DrawRectangle(goX + 10, goY + 10, 20, 20, {255, 100, 50, 150});
                    DrawRectangle(goX + goW - 30, goY + 10, 20, 20, {255, 100, 50, 150});
                    DrawRectangle(goX + 10, goY + goH - 30, 20, 20, {255, 100, 50, 150});
                    DrawRectangle(goX + goW - 30, goY + goH - 30, 20, 20, {255, 100, 50, 150});

                    EndDrawing();

                    if (IsKeyPressed(KEY_M))
                    {
                        stats.UpdateMaxDistance(maxDistanceThisRun);
                        stats.helicoptersDestroyed += enemies.GetActiveHelicopterCount();
                        stats.parrysPerformed += parrysThisRun;
                        stats.Save();

                        StopSound(gameAudio.engine);          // <--- AÑADIR
                        StopSound(gameAudio.heliRotor);       // <--- AÑADIR
                        StopMusicStream(gameAudio.gameMusic); // <--- AÑADIR

                        gameState = MENU;
                        selectedOption = 0;
                        break;
                    }

                    continue;
                }

                // --- DRAW ---
                BeginDrawing();

                // 1. CIELO (Gradiente Naranja - Atardecer)
                // Dibuja un rectángulo vertical degradado de naranja oscuro arriba a amarillo abajo
                DrawRectangleGradientV(0, 0, screenWidth, screenHeight,
                                       (Color){255, 140, 0, 255},    // Naranja oscuro (Top)
                                       (Color){255, 200, 100, 255}); // Amarillo suave (Bottom)

                // 2. SOL (Círculo estático)
                DrawCircle(screenWidth - 150, 150, 60, (Color){255, 255, 0, 200});   // Centro
                DrawCircle(screenWidth - 150, 150, 70, (Color){255, 255, 200, 100}); // Brillo externo

                // --- LÓGICA PARALLAX ---
                // Usamos la posición X del coche para calcular el desplazamiento
                float carX = myCar.GetPosition().x;
                float scaleFactor = 1.0f; // Ajusta si tus edificios se ven muy grandes/pequeños

                // ---------------------------------------------------------
                // CAPA 1: EDIFICIOS LEJANOS (Se mueven muy lento: 5% velocidad)
                // Color oscuro y siluetas simples para dar profundidad
                // ---------------------------------------------------------
                float farSpeed = 0.05f;
                float farOffset = -(carX * farSpeed);
                Color farColor = (Color){80, 40, 40, 255}; // Marrón muy oscuro/rojizo

                // Simulamos edificios infinitos usando módulo
                // Un "bloque" de edificios lejanos mide, digamos, 2000 pixeles
                float farPatternWidth = 2000.0f;
                float currentFarX = fmod(farOffset, farPatternWidth);

                // Dibujamos el patrón dos veces para cubrir la pantalla al hacer scroll
                for (int j = 0; j < 2; j++)
                {
                    float baseX = currentFarX + (j * farPatternWidth);

                    // Semilla fija para que los edificios siempre sean iguales en el mismo lugar
                    // (Usamos un truco simple: dibujamos rectángulos fijos relativos al baseX)

                    // Dibujamos 20 edificios por bloque
                    for (int i = 0; i < 20; i++)
                    {
                        // Generación pseudo-aleatoria basada en el índice 'i'
                        float h = 100.0f + (sinf(i * 132.0f) * 50.0f) + 150.0f; // Altura variable
                        float w = 80.0f + (cosf(i * 45.0f) * 30.0f);            // Ancho variable
                        float x = baseX + (i * 100.0f);                         // Posición

                        if (x + w > 0 && x < screenWidth)
                        { // Solo dibujar si está en pantalla
                            DrawRectangle(x, screenHeight - h - 50, w, h + 100, farColor);
                            // Ventanas lejanas (puntos pequeños)
                            if (i % 3 == 0)
                                DrawRectangle(x + 10, screenHeight - h + 20, 10, 10, (Color){255, 200, 100, 100});
                        }
                    }
                }

                // ---------------------------------------------------------
                // CAPA 2: EDIFICIOS MEDIOS (Se mueven más rápido: 20% velocidad)
                // Un poco más claros y detallados
                // ---------------------------------------------------------
                float midSpeed = 0.2f;
                float midOffset = -(carX * midSpeed);
                Color midColor = (Color){60, 30, 30, 255}; // Un poco más oscuro que el fondo lejano

                float midPatternWidth = 1500.0f;
                float currentMidX = fmod(midOffset, midPatternWidth);

                for (int j = 0; j < 2; j++)
                { // Dibujar 2 copias para el loop
                    float baseX = currentMidX + (j * midPatternWidth);

                    // Dibujamos 15 edificios por bloque
                    for (int i = 0; i < 15; i++)
                    {
                        float h = 150.0f + (cosf(i * 54.0f) * 80.0f) + 100.0f;
                        float w = 90.0f + (sinf(i * 22.0f) * 40.0f);
                        float x = baseX + (i * 120.0f);

                        if (x + w > 0 && x < screenWidth)
                        {
                            // Cuerpo edificio
                            DrawRectangle(x, screenHeight - h - 20, w, h + 100, midColor);

                            // Detalle: Borde iluminado por el atardecer (lado izquierdo)
                            DrawRectangle(x, screenHeight - h - 20, 5, h + 100, (Color){255, 100, 50, 50});

                            // Detalle: Ventanas iluminadas aleatorias
                            for (int wy = 0; wy < (int)(h / 30); wy++)
                            {
                                for (int wx = 0; wx < (int)(w / 25); wx++)
                                {
                                    // "Ruido" determinista para encender algunas luces
                                    if (((i + wx + wy) % 5) == 0)
                                    {
                                        DrawRectangle(x + 10 + (wx * 20), screenHeight - h + 20 + (wy * 30), 12, 18, (Color){255, 255, 150, 150});
                                    }
                                }
                            }
                        }
                    }
                }

                BeginMode2D(camera);

                terrain.Draw(tex.fuelCan, tex.mine, tex.ammo, tex.repair);
                enemies.Draw();
                myCar.Draw(tex.carChassis, tex.carWheel, tex.missile);
                explosionManager.Draw();

                for (const auto &line : parryLines)
                {
                    DrawLineEx(line.start, line.end, 15.0f, Fade(WHITE, line.alpha));
                    DrawLineEx(line.start, line.end, 5.0f, Fade(BLUE, line.alpha));
                }
                EndMode2D();

                // --- UI EN PANTALLA - Panel superior izquierdo ---
                int uiPanelX = 10;
                int uiPanelY = 10;
                int uiPanelW = 280;
                int uiPanelH = 200;

                // Marco UI
                DrawRectangle(uiPanelX - 2, uiPanelY - 2, uiPanelW + 4, uiPanelH + 4, {20, 200, 255, 150});
                DrawRectangle(uiPanelX, uiPanelY, uiPanelW, uiPanelH, Fade({30, 50, 80, 255}, 0.8f));

                // Título
                DrawText("[ DATOS ]", uiPanelX + 10, uiPanelY + 5, 16, {20, 255, 150, 255});
                DrawLineEx({(float)(uiPanelX + 5), (float)(uiPanelY + 25)}, {(float)(uiPanelX + uiPanelW - 5), (float)(uiPanelY + 25)}, 1, {20, 200, 255, 100});

                // Información
                DrawText(TextFormat("DISTANCIA: %.0f m", maxDistanceThisRun), uiPanelX + 10, uiPanelY + 35, 14, {20, 255, 200, 255});
                DrawText(TextFormat("HELICOPTEROS: %d", enemies.GetHelicoptersDestroyedCount()), uiPanelX + 10, uiPanelY + 55, 14, {255, 100, 50, 255});
                DrawText(TextFormat("PARRYS: %d", parrysThisRun), uiPanelX + 10, uiPanelY + 75, 14, {150, 100, 255, 255});

                // Barra de combustible
                DrawText("COMBUSTIBLE:", uiPanelX + 10, uiPanelY + 100, 12, {20, 255, 150, 255});
                DrawRectangleLines(uiPanelX + 10, uiPanelY + 115, 250, 15, {20, 200, 255, 150});
                DrawRectangle(uiPanelX + 12, uiPanelY + 117, myCar.GetFuel() * 2.46f, 11, {255, 100, 50, 255});

                // Munición
                DrawText(TextFormat("MISILES: %d/%d", myCar.GetAmmo(), myCar.GetMaxAmmo()), uiPanelX + 10, uiPanelY + 140, 14, {150, 200, 255, 255});

                // --- VIDAS (esquina superior derecha) ---
                int livesX = screenWidth - 120;
                int livesY = 10;
                DrawRectangle(livesX - 2, livesY - 2, 110, 80, {20, 200, 255, 150});
                DrawRectangle(livesX, livesY, 106, 76, Fade({30, 50, 80, 255}, 0.8f));
                DrawText("[ VIDAS ]", livesX + 10, livesY + 5, 14, {255, 100, 50, 255});
                DrawLineEx({(float)(livesX + 5), (float)(livesY + 20)}, {(float)(livesX + 100), (float)(livesY + 20)}, 1, {255, 100, 50, 100});

                for (int i = 0; i < myCar.GetMaxLives(); i++)
                {
                    Color lifeColor = (i < myCar.GetLives()) ? Color{255, 100, 50, 255} : Color{100, 50, 50, 150};
                    DrawCircle(livesX + 20 + (i * 18), livesY + 50, 8, lifeColor); // Cambié de 10 a 5
                }

                // --- TUTORIAL / CONTROLES DE JUEGO ---
                if (controlsTimer > 0.0f)
                {
                    // Configuración del panel de ayuda
                    int helpW = 340;
                    int helpH = 155;
                    int helpX = (screenWidth - helpW) / 2;
                    int helpY = 120;

                    // Colores temáticos
                    // Fondo oscuro azulado semitransparente
                    Color panelBg = Fade((Color){20, 30, 40, 255}, controlsAlpha * 0.9f);
                    // Borde cian neón
                    Color panelBorder = Fade((Color){0, 255, 255, 255}, controlsAlpha);
                    // Texto principal blanco brillante
                    Color textColor = Fade(WHITE, controlsAlpha);
                    // Texto de teclas resaltado (Amarillo/Naranja)
                    Color keyColor = Fade((Color){255, 200, 50, 255}, controlsAlpha);

                    // DIBUJAR PANEL
                    DrawRectangle(helpX, helpY, helpW, helpH, panelBg);
                    DrawRectangleLinesEx((Rectangle){(float)helpX, (float)helpY, (float)helpW, (float)helpH}, 2.0f, panelBorder);

                    // Línea decorativa superior
                    DrawLine(helpX + 10, helpY + 30, helpX + helpW - 10, helpY + 30, Fade(panelBorder, controlsAlpha * 0.5f));

                    // DIBUJAR TEXTO
                    int textX = helpX + 20;
                    int startY = helpY + 10;

                    // Título
                    DrawText("CONTROLES DE JUEGO", helpX + (helpW - MeasureText("CONTROLES DE JUEGO", 20)) / 2, startY, 20, panelBorder);

                    // Filas de controles
                    int rowH = 25;
                    int currentY = startY + 25;

                    // Función auxiliar local para dibujar líneas alineadas
                    auto DrawControlLine = [&](const char *keys, const char *action, int y)
                    {
                        DrawText(keys, textX, y, 16, keyColor);
                        // Alineamos la acción a la derecha
                        int actionW = MeasureText(action, 16);
                        DrawText(action, helpX + helpW - actionW - 20, y, 16, textColor);
                    };

                    DrawControlLine("A / D", "Frenar / Acelerar", currentY);
                    DrawControlLine("A / D", "Rotar en el aire", currentY + rowH);
                    DrawControlLine("ESPACIO", "Disparar Misil", currentY + rowH * 2);
                    DrawControlLine("SHIFT", "Escudo / Parry", currentY + rowH * 3);
                    DrawControlLine("ESC", "PAUSA", currentY + rowH * 4);
                }

                // --- ALERTA DE HELICÓPTERO ---
                if (enemyAlertTimer > 0.0f)
                {
                    float alertAlpha = (enemyAlertTimer > 1.0f) ? 1.0f : enemyAlertTimer;
                    int alertX = screenWidth / 2 - 200;
                    int alertY = 20;
                    int alertW = 400;
                    int alertH = 60;

                    DrawRectangle(alertX - 2, alertY - 2, alertW + 4, alertH + 4, Fade({255, 100, 50, 255}, alertAlpha));
                    DrawRectangle(alertX, alertY, alertW, alertH, Fade({100, 30, 20, 255}, alertAlpha * 0.7f));

                    const char *alertMsg = "!!! HELICOPTERO DETECTADO !!!";
                    int alertWidth = MeasureText(alertMsg, 18);
                    DrawText(alertMsg, screenWidth / 2 - alertWidth / 2, alertY + 15, 18, Fade({255, 200, 100, 255}, alertAlpha));
                }

                // --- ZONA DE PELIGRO ---
                if (maxDistanceThisRun > 6000)
                {
                    const char *dangerMsg = "[ ZONA DE PELIGRO ]";
                    int dangerWidth = MeasureText(dangerMsg, 24);
                    DrawText(dangerMsg, screenWidth / 2 - dangerWidth / 2, screenHeight - 50, 24, {255, 50, 50, 255});
                }

                // --- QUIZ (si está activo) ---
                if (myCar.IsQuizActive())
                {
                    Car::MathQuestion q = myCar.GetCurrentQuestion();
                    int panelX = screenWidth / 2 - 280;
                    int panelY = screenHeight - 240;
                    int panelWidth = 560;
                    int panelHeight = 220;

                    DrawRectangle(panelX - 2, panelY - 2, panelWidth + 4, panelHeight + 4, {150, 50, 255, 255});
                    DrawRectangle(panelX, panelY, panelWidth, panelHeight, Fade({60, 20, 80, 255}, 0.95f));

                    char questionStr[100];
                    sprintf(questionStr, "%d %c %d = ?", q.num1, q.operation, q.num2);
                    int textWidth = MeasureText(questionStr, 32);
                    DrawText(questionStr, panelX + (panelWidth - textWidth) / 2, panelY + 10, 32, {150, 100, 255, 255});
                    DrawText(TextFormat("Tiempo: %.1f", myCar.GetQuizTimer()), panelX + 20, panelY + 50, 16, {200, 150, 255, 255});

                    DrawLineEx({(float)(panelX + 20), (float)(panelY + 65)}, {(float)(panelX + panelWidth - 20), (float)(panelY + 65)}, 1, {150, 100, 255, 100});

                    for (int i = 0; i < 3; i++)
                    {
                        int optionX = panelX + 30 + (i * 170);
                        int optionY = panelY + 90;
                        int optionW = 150;
                        int optionH = 100;
                        Rectangle optionRect = {(float)optionX, (float)optionY, (float)optionW, (float)optionH};

                        Vector2 mousePos = GetMousePosition();
                        bool isHover = CheckCollisionPointRec(mousePos, optionRect);
                        Color color = isHover ? Color{200, 150, 255, 255} : Color{150, 100, 255, 200};

                        if (isHover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        {
                            myCar.AnswerQuestion(i);
                        }

                        DrawRectangle(optionX, optionY, optionW, optionH, Fade({80, 40, 120, 255}, 0.7f));
                        DrawRectangleLines(optionX, optionY, optionW, optionH, color);

                        char optionStr[30];
                        sprintf(optionStr, "%d", q.options[i]);
                        int optWidth = MeasureText(optionStr, 20);
                        DrawText(optionStr, optionX + (optionW - optWidth) / 2, optionY + 35, 20, color);
                    }
                }

                if (flashTimer > 0.0f)
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(WHITE, flashTimer));
                }

                EndDrawing();
            }
        }
    }

    UnloadTexture(tex.carChassis);
    UnloadTexture(tex.carWheel);
    UnloadTexture(tex.helicopter);
    UnloadTexture(tex.missile);
    UnloadTexture(tex.fuelCan);
    UnloadTexture(tex.mine);
    UnloadTexture(tex.ammo);
    UnloadTexture(tex.background);
    UnloadTexture(tex.repair);
    UnloadTexture(tex.explosionCarTex);
    UnloadTexture(tex.explosionHeliTex);
    UnloadTexture(tex.explosionMissileTex);
    UnloadTexture(tex.explosionMineTex);
    UnloadTexture(tex.menuBackground); // AGREGAR ESTA LÍNEA
    // DESCARGAR BOTONES
    UnloadTexture(tex.playOff);
    UnloadTexture(tex.playOn);
    UnloadTexture(tex.statsOff);
    UnloadTexture(tex.statsOn);
    UnloadTexture(tex.exitOff);
    UnloadTexture(tex.exitOn);
    UnloadTexture(tex.logo);

    UnloadTexture(tex.carChassis);
    // ... resto de texturas ...

    // Descargar audio
    gameAudio.Unload();
    CloseAudioDevice(); // <--- CERRAR AUDIO
    CloseWindow();

    CloseWindow();
    return 0;
}
