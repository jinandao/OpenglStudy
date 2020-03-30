#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 TexCoord;                                             
layout (location = 2) in vec3 Normal;                                               
                                                                                    
uniform mat4 gWVP;                                                                  
                                                                                    
out vec2 TexCoordOut;                                                               
                                                                                    
void main()                                                                         
{                                                                                   
    gl_Position = gWVP * vec4(Position, 1.0);                                       
	//gl_Position = vec4(Position.x-0.3,Position.y-0.3,Position.z-0.3, 1.0); 
    //gl_Position = vec4(Position.x,Position.y,0,1.0);  
	TexCoordOut = TexCoord;                                                         
}
