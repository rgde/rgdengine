const float PI=3.14159265f;

class vec3 {
public:
	union{
		struct{
			float x,y,z;
		};
		float v[3];
		};

    inline vec3() {x=y=z=0;}
    inline vec3(float f){ x=y=z=f; }
    inline vec3(float _x, float _y, float _z){ x=_x; y=_y; z=_z; }
    inline vec3(const float f[3]){ x=f[0]; y= f[1]; z=f[2]; }

	inline float operator[](int i) const{ return (&x)[i];}
	inline float& operator[](int i)		{return (&x)[i];	}


	inline vec3& operator += (const vec3& v){
	   x += v.x;   y += v.y;   z += v.z;
	   return *this;
	}

	inline vec3& operator -= (const vec3& v){
	   x -= v.x;   y -= v.y;   z -= v.z;
	   return *this;
	}

	inline vec3& operator *= (const vec3& v){
	   x *= v.x;   y *= v.y;   z *= v.z;
	   return *this;
	}

	inline vec3& operator /= (const vec3& v){
	   x /= v.x;   y /= v.y;   z /= v.z;
	   return *this;
	}

	inline vec3& operator *= (float s){
	   x *= s;   y *= s;   z *= s;
	   return *this;
	}

	inline vec3& operator /= (float s){
	   x /= s;   y /= s;   z /= s;
	   return *this;
	}

	inline friend vec3 operator + (const vec3& v){
	   return v;
	}

	inline friend vec3 operator - (const vec3& v){
	   return vec3(-v.x, -v.y, -v.z);
	}

	inline friend vec3 operator + (const vec3& v1, const vec3& v2){
	   return vec3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
	}

	inline friend vec3 operator - (const vec3& v1, const vec3& v2){
	   return vec3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
	}

	inline friend vec3 operator / (const vec3& v1, const vec3& v2){
	   return vec3(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z);
	}

	inline friend int operator < (const vec3& v1, const vec3& v2){
	   return v1[0] < v2[0] && v1[1] < v2[1] && v1[2] < v2[2];
	}

	inline friend int operator <= (const vec3& v1, const vec3& v2){
	   return v1[0] <= v2[0] && v1[1] <= v2[1] && v1[2] <= v2[2];
	}

	inline friend vec3 operator * (const vec3& v, float s){
	   return vec3(s*v.x, s*v.y, s*v.z);
	}

	inline friend vec3 operator * (float s, const vec3& v){
	   return vec3(s*v.x, s*v.y, s*v.z);
	}

	inline friend vec3 operator / (const vec3& v, float s){
	   return vec3(v.x/s, v.y/s, v.z/s);
	}

	inline friend int operator == (const vec3& v1, const vec3& v2){
	   return v1.x==v2.x && v1.y==v2.y && v1.z == v2.z;
	}

	inline friend float Magnitude (const vec3& v){
	   return (float) sqrtf(v.x*v.x+v.y*v.y+v.z*v.z);
	}

	inline friend float SquareMagnitude (const vec3& v){
	   return v.x*v.x + v.y*v.y + v.z*v.z;
	}

	inline friend vec3 Normalize (const vec3& v){
		float mag=1.0f/sqrtf(v.x*v.x+v.y*v.y+v.z*v.z);
		return v * mag;
	}

	inline void Normalize (){
		float mag=1.0f/sqrtf(x*x+y*y+z*z);
		x*=mag;
		y*=mag;
		z*=mag;
	}

	inline friend float Min (const vec3& v){
	   float ret = v.x;
	   if (v.y < ret) ret = v.y;
	   if (v.z < ret) ret = v.z;
	   return ret;
	}

	inline friend float Max (const vec3& v){
	   float ret = v.x;
	   if (ret < v.y) ret = v.y;
	   if (ret < v.z) ret = v.z;
	   return ret;
	}

	inline friend vec3 Minimize (const vec3& v1, const vec3& v2){
	   return vec3( v1[0] < v2[0] ? v1[0] : v2[0],
					   v1[1] < v2[1] ? v1[1] : v2[1],
					   v1[2] < v2[2] ? v1[2] : v2[2]);
	}

	inline friend vec3 Maximize (const vec3& v1, const vec3& v2){
	   return vec3( v1[0] > v2[0] ? v1[0] : v2[0],
					   v1[1] > v2[1] ? v1[1] : v2[1],
					   v1[2] > v2[2] ? v1[2] : v2[2]);
	}

	inline friend float operator * (const vec3& v1, const vec3& v2){
	   return v1.x*v2.x + v1.y * v2.y + v1.z*v2.z;
	}

	inline friend vec3 operator ^ (const vec3& v1, const vec3& v2){
		vec3 result;

		result[0] = v1[1] * v2[2] - v1[2] * v2[1];
		result[1] = v1[2] * v2[0] - v1[0] * v2[2];
		result[2] = v1[0] * v2[1] - v1[1] * v2[0];

		return result;
	}

	inline float GetPitch( void ) const {
		return atan2f( y, sqrtf( x*x + z*z ) );
	}

	inline float GetYaw( void ) const {
		return atan2f( z, x );
	}

	inline void SetYawPitch( float yaw, float pitch ){
		float p=cosf(pitch);
		x=p*cosf(yaw);
		z=p*sinf(yaw);		
		y=sinf(pitch);
	}

};