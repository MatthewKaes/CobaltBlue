cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    float4 transVector;
    float4 inputColor;
    float2 dimensions;
    float2 buffer;
};

struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
    float2 dimensions : TEXCOORD1;
};

PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
    input.position += transVector;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the input color for the pixel shader to use.
    output.color = input.color * inputColor;
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Store the dimension information.
    output.dimensions = dimensions;

    return output;
}
