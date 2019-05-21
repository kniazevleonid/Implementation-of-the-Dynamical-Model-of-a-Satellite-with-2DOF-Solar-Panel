uniform sampler2D u_texture;

uniform highp float u_lightPower;
varying highp vec4 v_position;
varying highp vec2 v_textcoord0;
varying highp vec3 v_normal;

uniform sampler2D u_shadowMap;

varying highp vec4 v_lightDirection;
varying highp vec4 v_positionLightMatrix;

float SampleShadowMap(sampler2D map, vec2 coords, float compare)
{
    vec4 v = texture2D(map, coords);
    float value = v.x * 255.0 + (v.y * 255.0 + (v.w + v.z * 255.0) / 255.0) / 255.0;
    return step(compare, value);

}

float CalcShadowAmount(sampler2D map, vec4 initShadowCoords)
{
    vec3 tmp = v_positionLightMatrix.xyz / v_positionLightMatrix.w;
    tmp = tmp * vec3(0.5) + vec3(0.5);
    return SampleShadowMap(u_shadowMap, tmp.xy, tmp.z * 255.0 - 0.75);
}

void main(void)
{
    highp float shadowCoef = CalcShadowAmount(u_shadowMap, v_positionLightMatrix);

    vec4 resultCol = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 eyePos = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 diffMatCol = texture2D(u_texture, v_textcoord0);
    vec3 eyeVec = normalize(v_position.xyz - eyePos.xyz);
    vec3 light =  normalize(v_lightDirection.xyz);
    vec3 reflectlight = normalize(reflect(light, v_normal));
    float len = length(v_position.xyz - eyePos.xyz);
    float specularFactor = 10.0;
    float ambientFactor = 0.1;

    vec4 diffcolor = diffMatCol * u_lightPower * max(0.0, dot(v_normal, -light)) / (1.0 + 0.25 * len * len);
    resultCol = diffcolor;
    vec4 ambient = ambientFactor * diffcolor;
    resultCol += ambient;
    vec4 specular = vec4(1.0, 1.0, 1.0, 1.0) * u_lightPower * pow(max(0.0, dot(reflectlight, -eyeVec)), specularFactor)/ (1.0 + 0.25 * len * len);

    resultCol += specular;

    shadowCoef += 0.2;

    if (shadowCoef > 1.0)
        shadowCoef = 1.0;

    gl_FragColor = resultCol * shadowCoef;
    //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
