#ifndef _HIGHER_ORDER_FUNCTION_DEMO_H
#define _HIGHER_ORDER_FUNCTION_DEMO_H

#include "generic_list.h"

int hofdemo_sum_of_odd_numbers(GenericList numbers);
void hofdemo_reset_every_fifth_counter(void);
GenericList hofdemo_extract_every_fifth(GenericList numbers);
GenericList hofdemo_group_words_by_initial(GenericList words);
GenericList hofdemo_order_words_lexicographically(GenericList words);
GenericList hofdemo_unique_words(GenericList words);

#endif