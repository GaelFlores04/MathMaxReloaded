#pragma once
#include <fstream>
#include <cmath>

struct GameStats
{
    float maxDistance = 0.0f;
    int helicoptersDestroyed = 0;
    int parrysPerformed = 0;
    
    // Guardar estadísticas en archivo
    void Save(const char* filename = "game_stats.dat")
    {
        std::ofstream file(filename, std::ios::binary);
        if (file.is_open())
        {
            file.write((char*)&maxDistance, sizeof(maxDistance));
            file.write((char*)&helicoptersDestroyed, sizeof(helicoptersDestroyed));
            file.write((char*)&parrysPerformed, sizeof(parrysPerformed));
            file.close();
        }
    }
    
    // Cargar estadísticas desde archivo
    void Load(const char* filename = "game_stats.dat")
    {
        std::ifstream file(filename, std::ios::binary);
        if (file.is_open())
        {
            file.read((char*)&maxDistance, sizeof(maxDistance));
            file.read((char*)&helicoptersDestroyed, sizeof(helicoptersDestroyed));
            file.read((char*)&parrysPerformed, sizeof(parrysPerformed));
            file.close();
        }
    }
    
    // Actualizar máxima distancia
    void UpdateMaxDistance(float currentDistance)
    {
        if (currentDistance > maxDistance)
            maxDistance = currentDistance;
    }
};
