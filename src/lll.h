#ifndef LLL_H
#define LLL_H

void update_bk(Vector *B_k, const int mu, const Vector *B_j, const int dim);
void LLL(Vector2D *B, const int dim);

#endif // LLL_H