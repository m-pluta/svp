#ifndef LLL_H
#define LLL_H

void update_bk(Vector *B_k, const long long mu, const Vector *B_j, const int dim);
void LLL(Vector2D *B, const int dim);

#endif // LLL_H