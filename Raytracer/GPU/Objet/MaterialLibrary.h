#ifndef MATERIAL_LIBRARY_H
#define MATERIAL_LIBRARY_H

#include "Material.h"

namespace MatLib {
    // Matériau Rouge Mat
    inline Material Red() {
        return Material(Vec3D(255, 0, 0), Vec3D(0.1, 0.1, 0.1), Vec3D(0.9, 0.9, 0.9), 
                        Vec3D(0.5, 0.5, 0.5), 64.0f, 0.0f, 0.0f, 1.0f);
    }

    // Verre Bleu 
    inline Material BlueGlass() {
        // La couleur est légère pour laisser passer la lumière
        return Material(Vec3D(50, 50, 255),
                        Vec3D(0.05, 0.05, 0.05), Vec3D(0.2, 0.2, 0.2), Vec3D(1.0, 1.0, 1.0),
                        128.0f, 0.1f, 0.9f, 1.5f);
    }
    
    //Star material
    inline Material Star_mat(){
        return Material(Vec3D(255.0f, 255.0f, 255.0f), 
                        Vec3D(1.0f, 1.0f, 1.0f),Vec3D(0.7f, 0.7f, 0.7f), Vec3D(0.3f, 0.3f, 0.3f),
                        32.0f, 0.0f, 0.0f, 1.0f);
    }

    // Chrome 
    inline Material Chrome() {
        return Material(Vec3D(255, 255, 255),
                        Vec3D(0.0, 0.0, 0.0), Vec3D(0.1, 0.1, 0.1), Vec3D(1.0, 1.0, 1.0),
                        256.0f, 0.95f, 0.0f, 1.0f);
    }

    // Sol
    inline Material Floor() {
        return Material(Vec3D(100, 100, 100),
                        Vec3D(0.1, 0.1, 0.1), Vec3D(0.8, 0.8, 0.8),Vec3D(0.2, 0.2, 0.2),
                        32.0f, 0.5f, 0.0f, 1.0f);
    }
}

#endif