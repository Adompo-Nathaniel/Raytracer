#ifndef MATERIAL_LIBRARY_H
#define MATERIAL_LIBRARY_H
 
#include "Material.h"
namespace MatLib {
 

    // Rouge plastique mat
    inline Material Red() {
        return Material(
            Vec3D(255.0f,  0.0f,  0.0f), 
            Vec3D(0.05f, 0.05f, 0.05f), 
            Vec3D(0.85f, 0.85f, 0.85f),   
            Vec3D(0.35f, 0.35f, 0.35f),  
            32.0f,                       
            0.0f,                       
            0.0f,                           
            1.0f                            
        );
    }

    inline Material Blue() {
        return Material(
            Vec3D(0.0f,  0.0f,  255.0f), 
            Vec3D(0.05f, 0.05f, 0.05f), 
            Vec3D(0.85f, 0.85f, 0.85f),   
            Vec3D(0.35f, 0.35f, 0.35f),  
            32.0f,                       
            0.0f,                       
            0.0f,                           
            1.0f                            
        );
    }
    inline Material Green() {
        return Material(
            Vec3D(0.0f,  255.0f,  0.0f), 
            Vec3D(0.05f, 0.05f, 0.05f), 
            Vec3D(0.85f, 0.85f, 0.85f),   
            Vec3D(0.35f, 0.35f, 0.35f),  
            32.0f,                       
            0.0f,                       
            0.0f,                           
            1.0f                            
        );
    }
    inline Material Black() {
        return Material(
            Vec3D(0.0f,  0.0f,  0.0f), 
            Vec3D(0.05f, 0.05f, 0.05f), 
            Vec3D(0.85f, 0.85f, 0.85f),   
            Vec3D(0.35f, 0.35f, 0.35f),  
            32.0f,                       
            0.0f,                       
            0.0f,                           
            1.0f                            
        );
    }
    inline Material White() {
        return Material(
            Vec3D(255.0f,  255.0f,  255.0f), 
            Vec3D(0.05f, 0.05f, 0.05f), 
            Vec3D(0.85f, 0.85f, 0.85f),   
            Vec3D(0.35f, 0.35f, 0.35f),  
            32.0f,                       
            0.0f,                       
            0.0f,                           
            1.0f                            
        );
    }
    // Chrome (miroir quasi-parfait)
    inline Material Chrome() {
        return Material(
            Vec3D(0.0f, 0.0f, 0.0f),
            Vec3D(0.0f, 0.0f, 0.0f),      
            Vec3D(0.0f, 0.0f, 0.0f),      
            Vec3D(0.85f, 0.88f, 0.91f),   
            255.0f,                       
            0.98f,                        
            0.0f,
            1.0f
        );
    }
 
    // Or 
    inline Material Gold() {
        return Material(
            Vec3D(0.0f, 0.0f, 0.0f),      
            Vec3D(0.0f, 0.0f, 0.0f),      
            Vec3D(0.0f, 0.0f, 0.0f),      
            Vec3D(1.0, 0.85f, 0.4f),    
            255.0f,                       
            0.95f,                        
            0.0f,
            1.0f
        );
    }
 

    // Acier brossé métal
    inline Material Steel() {
        return Material(
            Vec3D(0.0f, 0.0f, 0.0f),
            Vec3D(0.0f, 0.0f, 0.0f),    
            Vec3D(0.0f, 0.0f, 0.0f),    
            Vec3D(0.937f, 0.749f, 0.50f),
            255.0f,                    
            0.8f,                      
            0.0f,
            1.0f
        );
    }
 
    // Verre 
    inline Material BlueGlass() {
        return Material(
            Vec3D(210.0f, 230.0f, 255.0f), 
            Vec3D(0.01f,  0.01f,  0.02f),
            Vec3D(0.05f,  0.07f,  0.1f),   
            Vec3D(1.0f,   1.0f,   1.0f),    
            256.0f,                     
            0.1f,                           
            0.9f,                          
            1.5f                            
        );
    }
 
    // Étoile
    inline Material Star_mat() {
        return Material(
            Vec3D(255.0f, 255.0f, 255.0f),
            Vec3D(1.0f,   1.0f,   1.0f), 
            Vec3D(0.0f,   0.0f,   0.0f),  
            Vec3D(0.0f,   0.0f,   0.0f),   
            1.0f,
            0.0f,
            0.0f,
            1.0f
        );
    }
 
    // Sol 
    inline Material Floor() {
        return Material(
            Vec3D(200.0f,  200.0f,  200.0f), 
            Vec3D(0.04f,  0.04f,  0.04f),  
            Vec3D(0.55f,  0.55f,  0.58f),  
            Vec3D(0.45f,  0.45f,  0.5f),   
            64.0f,                         
            0.12f,
            0.0f,
            1.0f
        );
    }
    inline Material TableTop() {
        return Material(
            Vec3D(180.0f, 165.0f, 140.0f),
            Vec3D(0.03f, 0.03f, 0.02f),
            Vec3D(0.30f, 0.28f, 0.24f),
            Vec3D(0.15f, 0.13f, 0.10f),
            16.0f,
            0.05f,
            0.0f,
            1.0f
        );
    }
}
 
#endif