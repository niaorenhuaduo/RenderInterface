/********************************************************************/
// I32
/********************************************************************/
inline IntRect::IntRect() : 
left  (0), 
top   (0), 
width (0), 
height(0) {}



inline IntRect::IntRect(I32 _left, I32 _top, I32 _width, I32 _height) : 
left  (_left), 
top   (_top), 
width (_width), 
height(_height) {}


/********************************************************************/
// FLOAT
/********************************************************************/
inline FloatRect::FloatRect() : 
left  (0.0f), 
top   (0.0f), 
width (0.0f), 
height(0.0f) {}



inline FloatRect::FloatRect(float _left, float _top, float _width, float _height) : 
left  (_left), 
top   (_top), 
width (_width), 
height(_height) {}




