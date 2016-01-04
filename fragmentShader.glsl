/*
Brian Dunham
Comp 465
Professor Barnes
brian.dunham.747@my.csun.edu
*/

# version 330 core

in vec4 fColor;
in vec3 fPosition;
in vec3 fNormal;

out vec4 fragment_color;

uniform vec4 PointLightLocation;
uniform vec3 KA;  //Ambient
uniform vec3 KD;  //Diffuse
uniform vec3 KS;  //Specular
uniform vec4 ToggleLights;

uniform vec3  SpotLightLocation;
uniform vec3  SpotLightDirection;
uniform float SpotLightAngle;

vec4 pointLight(){

    vec3 n = normalize(fNormal);
    vec3 at = vec3(PointLightLocation) - fPosition;
    vec3 s = normalize(at);
    vec3 v = normalize(vec3(-fPosition));
    vec3 h = normalize(v + s);

    //Local variables
    vec3 ka = vec3(.2,.2,.2);
    vec3 kd = vec3(0,0,0);
    vec3 ks = vec3(0,0,0);
    float dist = sqrt(pow(at.x, 2) + pow(at.y, 2) + pow(at.z, 2));
	float intensity = ((10000 - dist + 2000)/10000);

    //Ambient
    ka = KA;

    //Diffuse
    kd.x = max(intensity, KD.x);
    kd.y = max(intensity, KD.y);
    kd.z = max(intensity, KD.z);

    //Specular
    ks.x = min(intensity, KS.x);
    ks.y = min(intensity, KS.y);
    ks.z = min(intensity, KS.z);

    return fColor *
		vec4(ka + kd * max(dot(s,n),0.0), 1.0) +
		vec4(ks * pow(max(dot(h,n),0.0), 2) , 1.0);
}

vec4 spotLight(){

    vec3 n = normalize(fNormal);
    vec3 at = (SpotLightLocation - fPosition);
	vec3 s = normalize(at);
    vec3 v = normalize(vec3(-fPosition));
    vec3 h = normalize(s + v);

    vec3 kd = vec3(1,1,1);
    vec3 ks = vec3(0,0,0);

    //Get the angle between the spot light and the fragment
    float angle = max(acos(dot(normalize(SpotLightDirection), -(s))), 0);

    //Calculate light affects if fragment is within spot light angle
    if(angle <= (SpotLightAngle)){

        //Debug
        if(ToggleLights.w != 0)
            return vec4(1,1,1,1);

        return fColor * 
			vec4(kd * max(dot(s,n),0.0), 1.0) +
			vec4(ks * pow(max(dot(h,n),0.0), 2) , 1.0);
    }

    //Else fragment not affected by spotlight
    return vec4(0,0,0,0);
}

void main(){

    fragment_color = (vec4(KA, 1) * fColor);

    //Add ambient light effects
    if(ToggleLights.x == 0)
		fragment_color = vec4(0,0,0,0);

    //Add point light effects
    if(ToggleLights.y != 0)
        fragment_color += pointLight();
	
	//Add spot light effects
	if(ToggleLights.z != 0)
        fragment_color += spotLight();

}