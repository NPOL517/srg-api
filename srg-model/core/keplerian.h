#ifndef KEPLERIAN_H
#define KEPLERIAN_H

// Перевод декакртовых координат в кеплеровы элементы орбиты.
void cas2kep(const double cas[6], double kep[6]);

// Первод кеплеровых элементов орбиты в декартовы координаты.
void kep2cas(const double kep[6], double cas[6]);

#endif /* KEPLERIAN_H */
