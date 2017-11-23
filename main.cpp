#include <iostream>
#include "SSLL.h"
#include "PSLL.h"
#include "CDAL.h"
#include "SDAL.h"
#include "CBL.h"

int main() {

    cop3530::SSLL<int> SSLL_List;
    cop3530::PSLL<int> PSLL_List;
    cop3530::SDAL<int> SDAL_List;
    cop3530::CBL<int> CBL_List;
    cop3530::CDAL<int> CDAL_List;





    /// test for const iterators
    /*
     *
     *    /// iterators test
    cop3530::CDAL<char> list;
    list.push_front( 'X' );
     *   cop3530::CDAL<char> const const_list = list;
     *   cop3530::CDAL<char>::iterator iter = list.Begin();
    std::cout << *iter << std::endl;        // prints an X
    *iter = 'Y';
    std::cout << *iter << std::endl;        // prints a Y
    iter++;
    *iter = 'W';
    std::cout << *iter << std::endl;        // prints a Y

    cop3530::CDAL<char>::const_iterator const_iter = const_list.Begin();
    std::cout << *const_iter << std::endl;  // prints an X
  // *const_iter = 'Z';
     * */



    return 0;
}