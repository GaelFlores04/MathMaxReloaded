/*
 * Proyecto: MATHMAX - RELOADED
 * Archivo: stats.h
 * Proposito: validar, acumular y conservar estadisticas entre ejecuciones.
 * Requisitos: RF10, RNF04.
 * Base POO: Gael Flores Canedo / Israel Gomez Ahumada.
 * Adaptacion V&V: Gael Flores / Jorge Roel Vasquez; asistencia de Codex.
 * Revision: 2026-11-09
 */
#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <limits>
#include <string>

struct GameStats
{
    float maxDistance = 0.0f;
    int helicoptersDestroyed = 0;
    int parrysPerformed = 0;
    std::string lastError;
    static constexpr std::uint32_t FILE_MAGIC = 0x4D4D5231;
    static constexpr std::uint32_t FILE_VERSION = 1;
    static constexpr std::streamoff LEGACY_SIZE = 12;
    static constexpr std::streamoff VERSIONED_SIZE = 24;

    bool IsValid() const
    {
        return std::isfinite(maxDistance) && maxDistance >= 0.0f && helicoptersDestroyed >= 0 &&
               parrysPerformed >= 0;
    }

    /** RF10: el llamador garantiza una sola acumulacion por partida. Distancia en metros. */
    bool AccumulateRun(float distanceMeters, int destroyedCount, int parryCount)
    {
        if (!IsValid() || !std::isfinite(distanceMeters) || distanceMeters < 0.0f ||
            destroyedCount < 0 || parryCount < 0 ||
            destroyedCount > std::numeric_limits<int>::max() - helicoptersDestroyed ||
            parryCount > std::numeric_limits<int>::max() - parrysPerformed)
        {
            lastError = "ESTADISTICAS_INVALIDAS";
            return false;
        }
        maxDistance = std::max(maxDistance, distanceMeters);
        helicoptersDestroyed += destroyedCount;
        parrysPerformed += parryCount;
        lastError.clear();
        return true;
    }

    // Guardar estadísticas en archivo
    bool Save(const char *filename = "game_stats.dat")
    {
        static_assert(sizeof(float) == 4 && sizeof(int) == 4,
                      "El formato requiere float e int de 32 bits");
        if (!IsValid())
        {
            lastError = "ESTADISTICAS_INVALIDAS";
            return false;
        }
        const std::filesystem::path destination(filename);
        // shortcut: un escritor por archivo; agregar bloqueo si se admiten instancias concurrentes.
        const std::filesystem::path temporary = destination.string() + ".tmp";
        std::ofstream file(temporary, std::ios::binary | std::ios::trunc);
        if (!file)
        {
            lastError = "NO_SE_PUDO_ABRIR_TEMPORAL";
            return false;
        }
        const std::uint32_t magic = FILE_MAGIC, version = FILE_VERSION;
        const std::uint32_t payloadBytes = LEGACY_SIZE;
        file.write(reinterpret_cast<const char *>(&magic), sizeof(magic));
        file.write(reinterpret_cast<const char *>(&version), sizeof(version));
        file.write(reinterpret_cast<const char *>(&payloadBytes), sizeof(payloadBytes));
        file.write(reinterpret_cast<const char *>(&maxDistance), sizeof(maxDistance));
        file.write(reinterpret_cast<const char *>(&helicoptersDestroyed),
                   sizeof(helicoptersDestroyed));
        file.write(reinterpret_cast<const char *>(&parrysPerformed), sizeof(parrysPerformed));
        file.flush();
        const bool wroteAll = file.good();
        file.close();
        std::error_code error;
        if (!wroteAll || file.fail())
        {
            std::filesystem::remove(temporary, error);
            lastError = "ESCRITURA_INCOMPLETA";
            return false;
        }
        std::filesystem::rename(temporary, destination, error);
        if (error)
        {
            std::filesystem::remove(temporary, error);
            lastError = "NO_SE_PUDO_REEMPLAZAR_ARCHIVO";
            return false;
        }
        lastError.clear();
        return true;
    }

    // Cargar estadísticas desde archivo
    bool Load(const char *filename = "game_stats.dat")
    {
        std::error_code error;
        if (!std::filesystem::exists(filename, error))
        {
            lastError = error ? "NO_SE_PUDO_CONSULTAR_ARCHIVO" : "";
            return !error;
        }
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file)
        {
            lastError = "NO_SE_PUDO_ABRIR_ARCHIVO";
            return false;
        }
        const auto size = file.tellg();
        if (size != LEGACY_SIZE && size != VERSIONED_SIZE)
        {
            lastError = "TAMANO_DE_ARCHIVO_INVALIDO";
            return false;
        }
        file.seekg(0);
        if (size == VERSIONED_SIZE)
        {
            std::uint32_t magic = 0, version = 0, payloadBytes = 0;
            file.read(reinterpret_cast<char *>(&magic), sizeof(magic));
            file.read(reinterpret_cast<char *>(&version), sizeof(version));
            file.read(reinterpret_cast<char *>(&payloadBytes), sizeof(payloadBytes));
            if (!file || magic != FILE_MAGIC || version != FILE_VERSION ||
                payloadBytes != LEGACY_SIZE)
            {
                lastError = "CABECERA_DE_ARCHIVO_INVALIDA";
                return false;
            }
        }
        GameStats loaded;
        file.read(reinterpret_cast<char *>(&loaded.maxDistance), sizeof(loaded.maxDistance));
        file.read(reinterpret_cast<char *>(&loaded.helicoptersDestroyed),
                  sizeof(loaded.helicoptersDestroyed));
        file.read(reinterpret_cast<char *>(&loaded.parrysPerformed),
                  sizeof(loaded.parrysPerformed));
        if (!file || !loaded.IsValid())
        {
            lastError = "DATOS_DE_ARCHIVO_INVALIDOS";
            return false;
        }
        maxDistance = loaded.maxDistance;
        helicoptersDestroyed = loaded.helicoptersDestroyed;
        parrysPerformed = loaded.parrysPerformed;
        lastError.clear();
        return true;
    }

    // Actualizar máxima distancia
    void UpdateMaxDistance(float currentDistance)
    {
        if (std::isfinite(currentDistance) && currentDistance >= 0.0f)
        {
            maxDistance = std::max(maxDistance, currentDistance);
        }
    }
};
