#pragma once

constexpr float pi {3.141592653589793238462643383f};

void center_origin(sf::Sprite& ssprite);
void center_origin(sf::Text& ttext);
class Animation;
void center_origin(Animation& aanimation);
float to_radian(float ddegree);
float to_degree(float rradian);
float vector_length(sf::Vector2f vvector);
sf::Vector2f unit_vector(sf::Vector2f vvector);
int random_int(int mmax);
