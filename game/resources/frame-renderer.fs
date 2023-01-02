#version 330

#define TEXTURE_SIZE 12
#define TILE_SIZE 4

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

uniform vec2 spriteSize;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

void main()
{
    vec2 newPos = fragTexCoord;

     vec2 pixelPos = vec2((fragTexCoord.x * spriteSize.x), (fragTexCoord.y * spriteSize.y));
    
    if (pixelPos.x > TILE_SIZE) {
        if (pixelPos.x < spriteSize.x - TILE_SIZE) {
            int pixel = (int(pixelPos.x) - TILE_SIZE) % TILE_SIZE;
            newPos.x = float(TILE_SIZE + pixel + 1) / float(TEXTURE_SIZE);
        } else {
            newPos.x = (spriteSize.x - pixelPos.x) / float(TEXTURE_SIZE);
        }
    } else {
        newPos.x = pixelPos.x / float(TEXTURE_SIZE);
    }

    if (pixelPos.y > TILE_SIZE) {
        if (pixelPos.y < spriteSize.y - TILE_SIZE) {
            int pixel = (int(pixelPos.y) - TILE_SIZE) % TILE_SIZE;
            newPos.y = float(TILE_SIZE + pixel + 1) / float(TEXTURE_SIZE);
        } else {
            newPos.y = (spriteSize.y - pixelPos.y) / float(TEXTURE_SIZE);
        }
    } else {
        newPos.y = pixelPos.y / float(TEXTURE_SIZE);
    }


   vec4 texelColor = texture(texture0, newPos)*colDiffuse*fragColor;
   finalColor = texelColor;
}