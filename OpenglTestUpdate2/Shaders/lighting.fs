#version 330

in vec2 TexCoord0;
//in vec3 Normal0;                                                                    
                                                                                    
out vec4 FragColor;                                                                 
                                                                                    
struct DirectionalLight                                                             
{                                                                                   
    vec3 Color;                                                                     
    float AmbientIntensity;                                                         
    float DiffuseIntensity;                                                         
    vec3 Direction;                                                                 
};                                                                                  
                                                                                    
uniform DirectionalLight gDirectionalLight;                                         
uniform sampler2D gSampler;                                                  
uniform sampler2D gShadowMap;       
                                                                                    
void main()                                                                         
{                                                                                   
    //vec4 AmbientColor = vec4(gDirectionalLight.Color, 1.0f) *                       
    //                    gDirectionalLight.AmbientIntensity;                         
                                                                                    
    //float DiffuseFactor = dot(normalize(Normal0), -gDirectionalLight.Direction);    
                                                                                    
    //vec4 DiffuseColor;                                                              
                                                                                    
    //if (DiffuseFactor > 0) {                                                        
    //    DiffuseColor = vec4(gDirectionalLight.Color, 1.0f) *                        
    //                  gDirectionalLight.DiffuseIntensity *                         
    //                   DiffuseFactor;                                               
    //}                                                                               
    //else {                                                                          
    //    DiffuseColor = vec4(0, 0, 0, 0);                                            
    //}                                                                               
                                                                                    
    //FragColor = texture2D(gSampler, TexCoord0.xy) *                                 
    //            (AmbientColor + DiffuseColor)+vec4(1.0,1.0,1.0,1.0);  
	FragColor=0.5*texture2D(gShadowMap, TexCoord0.xy)+0.5*texture2D(gSampler, TexCoord0.xy);
	//FragColor=texture2D(gSampler, TexCoord0.xy);
	//FragColor=texture2D(gShadowMap, TexCoord0.xy);
	//FragColor=vec4(1.0,1.0,1.0,1.0);
}