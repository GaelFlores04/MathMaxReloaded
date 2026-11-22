/*
 * Proyecto: MATHMAX - RELOADED
 * Archivo: tests/test_game.cpp
 * Proposito: regresion de retos, recursos y persistencia. RF04, RF06, RF07, RF10, RNF04.
 * Adaptacion V&V: Gael Flores / Jorge Roel Vasquez; asistencia de Codex.
 * Revision: 2026-11-09
 */
#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>

#include "carb.h"
#include "fisicaM.h"
#include "stats.h"

void WriteLegacy(const std::filesystem::path &path, float meters, int destroyed, int parrys)
{
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    file.write(reinterpret_cast<const char *>(&meters), sizeof(meters));
    file.write(reinterpret_cast<const char *>(&destroyed), sizeof(destroyed));
    file.write(reinterpret_cast<const char *>(&parrys), sizeof(parrys));
    assert(file.good());
}

int main()
{
    SetTraceLogLevel(LOG_WARNING);
    SetRandomSeed(20261109);
    PhysicsWorld physics;
    Car car(physics.GetWorldId(), 0, 300);
    bool sawZero = false, sawNegative = false, sawDivision = false;
    for (const float meters : {0.0f, 2000.0f, 4000.0f, 6000.0f})
    {
        for (int question = 0; question < 5000; ++question)
        {
            car.GenerateMathQuestion(meters);
            const auto value = car.GetCurrentQuestion();
            assert(value.options[0] != value.options[1]);
            assert(value.options[0] != value.options[2]);
            assert(value.options[1] != value.options[2]);
            int correctCount = 0;
            for (int option : value.options)
            {
                correctCount += option == value.correctAnswer;
            }
            assert(correctCount == 1);
            int expected = 0;
            switch (value.operation)
            {
            case '+':
                expected = value.num1 + value.num2;
                break;
            case '-':
                expected = value.num1 - value.num2;
                break;
            case '*':
                expected = value.num1 * value.num2;
                break;
            case '/':
                assert(value.num2 != 0 && value.num1 % value.num2 == 0);
                expected = value.num1 / value.num2;
                sawDivision = true;
                break;
            default:
                assert(false);
            }
            assert(expected == value.correctAnswer);
            sawZero = sawZero || expected == 0;
            sawNegative = sawNegative || expected < 0;
            if (meters < 2000.0f)
            {
                assert(value.operation == '+');
            }
            else if (meters < 4000.0f)
            {
                assert(value.operation == '-');
            }
            else if (meters < 6000.0f)
            {
                assert(value.operation == '*');
            }
        }
    }
    assert(sawZero && sawNegative && sawDivision);
    std::cout << "PASS P01: 20000 preguntas, 4 operaciones, cero, negativos y opciones unicas\n";

    car.LoseLife();
    assert(car.GetLives() == 4);
    car.ActivateQuiz(0);
    car.AnswerQuestion(-1);
    car.AnswerQuestion(3);
    assert(car.IsQuizActive() && car.GetLives() == 4);
    auto question = car.GetCurrentQuestion();
    int correct = 0;
    while (question.options[correct] != question.correctAnswer)
    {
        ++correct;
    }
    car.AnswerQuestion(correct);
    assert(!car.IsQuizActive() && car.GetLives() == 5);
    car.AnswerQuestion(correct);
    assert(car.GetLives() == 5);
    std::cout << "PASS P02: indices invalidos y recompensa unica con maximo de 5 vidas\n";

    car.ActivateQuiz(2000);
    question = car.GetCurrentQuestion();
    int wrong = 0;
    while (question.options[wrong] == question.correctAnswer)
    {
        ++wrong;
    }
    car.AnswerQuestion(wrong);
    assert(car.GetLives() == 5 && !car.IsQuizActive());
    car.ActivateQuiz(6000);
    car.UpdateQuiz(10.1f);
    assert(!car.IsQuizActive() && car.GetLives() == 5);
    std::cout << "PASS P03: respuesta incorrecta y expiracion no recuperan vidas\n";

    assert(car.GetAmmo() == 0);
    car.Shoot();
    assert(car.GetAmmo() == 0);
    car.AddAmmo(100);
    assert(car.GetAmmo() == 6);
    car.AddAmmo(-10);
    assert(car.GetAmmo() == 6);
    car.Shoot();
    assert(car.GetAmmo() == 5);
    car.AddFuel(-10);
    car.AddFuel(std::numeric_limits<float>::quiet_NaN());
    car.AddFuel(1000);
    assert(car.GetFuel() == 100.0f);
    std::cout << "PASS P04: limites de municion, consumo de disparo y entradas de combustible\n";

    const auto dir = std::filesystem::path("build/tests_data");
    std::filesystem::create_directories(dir);
    const auto versioned = (dir / "versioned.dat").string();
    GameStats stats;
    assert(stats.AccumulateRun(1000, 2, 1));
    assert(stats.AccumulateRun(200, 0, 0));
    assert(stats.maxDistance == 1000 && stats.helicoptersDestroyed == 2);
    assert(stats.Save(versioned.c_str()));
    assert(stats.Save(versioned.c_str()));
    assert(std::filesystem::file_size(versioned) == GameStats::VERSIONED_SIZE);
    GameStats restored;
    assert(restored.Load(versioned.c_str()));
    assert(restored.maxDistance == 1000 && restored.helicoptersDestroyed == 2 &&
           restored.parrysPerformed == 1);
    std::cout << "PASS P05: guardar, reemplazar y cargar formato versionado; record no disminuye\n";

    const auto legacy = (dir / "legacy.dat").string();
    WriteLegacy(legacy, 123.5f, 7, 3);
    assert(restored.Load(legacy.c_str()));
    assert(restored.maxDistance == 123.5f && restored.helicoptersDestroyed == 7);
    assert(std::filesystem::file_size(legacy) == 12);
    assert(restored.Save(legacy.c_str()));
    assert(std::filesystem::file_size(legacy) == 24);
    std::cout << "PASS P06: archivo legado de 12 bytes y migracion al guardar\n";

    const auto corrupt = (dir / "corrupt.dat").string();
    for (int length = 0; length < 24; ++length)
    {
        if (length == 12)
        {
            continue;
        }
        std::ofstream file(corrupt, std::ios::binary | std::ios::trunc);
        file << std::string(length, 'X');
        file.close();
        assert(!restored.Load(corrupt.c_str()));
        assert(restored.maxDistance == 123.5f && restored.helicoptersDestroyed == 7);
    }
    WriteLegacy(corrupt, std::numeric_limits<float>::quiet_NaN(), 2, 1);
    assert(!restored.Load(corrupt.c_str()));
    WriteLegacy(corrupt, -1, 2, 1);
    assert(!restored.Load(corrupt.c_str()));
    WriteLegacy(corrupt, 1, -2, 1);
    assert(!restored.Load(corrupt.c_str()));
    std::ofstream badHeader(corrupt, std::ios::binary | std::ios::trunc);
    badHeader << std::string(24, 'X');
    badHeader.close();
    assert(!restored.Load(corrupt.c_str()));
    std::cout << "PASS P07: archivos truncados, NaN, negativos y cabecera invalida rechazados\n";

    assert(!restored.Save("build/directorio_inexistente/datos.dat"));
    assert(!restored.lastError.empty());
    assert(!restored.AccumulateRun(-1, 1, 1));
    restored.helicoptersDestroyed = std::numeric_limits<int>::max();
    assert(!restored.AccumulateRun(100, 1, 0));
    restored.helicoptersDestroyed = 7;
    assert(restored.Load("build/no_existe_actividad_10.dat"));
    assert(restored.maxDistance == 123.5f && restored.helicoptersDestroyed == 7);
    std::cout << "PASS P08: escritura fallida, desbordamiento e inicio sin archivo\n";
    std::cout << "RESULTADO: 8 grupos aprobados; codigo de salida 0\n";
}
