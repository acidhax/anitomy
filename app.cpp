#include <iostream>
#include "anitomy/anitomy.h"

int main() {
  anitomy::Anitomy anitomy;
  anitomy.Parse(L"[Ouroboros]_Fullmetal_Alchemist_Brotherhood_-_01.mkv");

  // auto& elements = anitomy.elements();

  // // Elements are iterable, where each element is a category-value pair
  // for (auto& element : elements) {
  //   std::wcout << element.first << L"\t" << element.second << std::endl;
  // }
  // std::wcout << std::endl;

  // // You can access values directly by using get() and get_all() methods
  // std::wcout << elements.get(anitomy::kElementAnimeTitle) << L" #" <<
  //               elements.get(anitomy::kElementEpisodeNumber) << L" by " <<
  //               elements.get(anitomy::kElementReleaseGroup) << std::endl;

  return 0;
}
