import numpy as np
import math

def inner_product(v1, v2):
    return sum([e[0] * e[1] for e in zip(v1, v2)])

def norm(v):
    return math.sqrt(inner_product(v, v))


def gram_schmidt(B, N):
    mu = [[None] * N for i in range(N)]
    Bs = [[None] * N for i in range(N)]

    for i in range(N):
        for j in range(N):
            mu[i][j] = 0.0
            Bs[i][j] = float(B[i][j])
        for k in range(i):
            mu[i][k] = inner_product(B[i], Bs[k]) / inner_product(Bs[k], Bs[k])
            for j in range(N):
                Bs[i][j] -= mu[i][k] * Bs[k][j]

    return mu, Bs

def schorr_euchner(B, R, N):
    R_2 = R ** 2
    mu, Bs = gram_schmidt(B, N)
    
    p = [0] * (N + 1)
    v = [1 if i == 0 else 0 for i in range(N)]
    c = [0] * N
    w = [0] * N

    k, last_non_zero = 0, 0

    while True:
        p[k] = p[k+1] + ((v[k] - c[k]) ** 2) * inner_product(Bs[k], Bs[k])
        
        if p[k] < R_2:
            if k == 0:
                R_2 = p[k]
                # s = weighted_sum(B, v)
            else:
                k -= 1
                c[k] = -1 * sum([mu[i][k] * v[i] for i in range(k+1, N)]) 
                v[k] = round(c[k])
                w[k] = 1
        else:
            k += 1
            if k == N:
                return math.sqrt(R_2)
                # return norm(s)
            
            if k >= last_non_zero:
                last_non_zero = k
                v[k] += 1
            else:
                if v[k] > c[k]:
                    v[k] -= w[k]
                else:
                    v[k] += w[k]
                w[k] += 1


def weighted_sum(vectors, coefficients):
    vectors = np.array(vectors)
    coefficients = np.array(coefficients)

    result = np.sum(vectors * coefficients[:, np.newaxis], axis=0)

    return result.tolist()       

if __name__ == '__main__':   
    # v1 = [1, 1, 0]
    # v2 = [-1, 2, 1]
    # v3 = [3, -1, 2]
    # B = [v1, v2, v3]

    # v1 = [1629261209715472616, 1, 0]
    # v2 = [1063530182757434852, 0, 1]
    # v3 = [1721705031183157005, 0, 0]
    # B = [v1, v2, v3]

    # v1 = [87, 247]
    # v2 = [147, 122]
    # B = [v1, v2]

    v1 = [3462528728, 444363041, 953498107, 3019162, 293072558, 454157756, 1950819278, 2174448309, 2956307025, 2050440410]
    v2 = [1676991435, 2190045933, 3360605833, 3782131535, 1076691369, 1286267085, 3124954673, 1356441824, 1675223571, 3617515077]
    v3 = [3467837865, 2727897666, 2512785299, 3013662465, 108820214, 2883385007, 1068246967, 798890469, 2874893319, 279879480]
    v4 = [3036791821, 956443849, 199347373, 1643597530, 1622084324, 471638746, 1055553070, 2020155791, 1772672903, 179014643]
    v5 = [3726309520, 3416039174, 816778328, 4178510478, 1052560161, 2365858231, 3810589638, 1478643591, 3537405930, 1196123656]
    v6 = [2101281032, 731853286, 35290828, 1463099681, 62123405, 1905949073, 3166373256, 2899371508, 674265594, 3891491225]
    v7 = [2924313520, 4018395377, 1444789986, 2465339761, 1288445553, 3964167277, 3710424026, 2819673876, 103363586, 3597674584]
    v8 = [3237227071, 578279073, 119471827, 325475776, 1510452858, 210663454, 2633248087, 2140229824, 153673582, 2811110910]
    v9 = [1566590271, 1264942241, 750770120, 1308334483, 1808072791, 1483343336, 4170849932, 3261787946, 1391377726, 2865208703]
    v10 = [1950175842, 47002310, 377036287, 1445470496, 1706137745, 2166273829, 1896680823, 2831856618, 2825133614, 3610138421]
    B = [v1, v2,v3,v4,v5,v6,v7,v8,v9,v10]

    N = len(B)

    shortest = schorr_euchner(B, 100_000_000_000, N)
    print(shortest)
