/*
Main file for part 2a of the Project #2
Author: Marco Valero
File Title: query_tree.cc

Description: This file parses an input file "rebase210.txt" and creates a AVL tree from data from the file. Each node of the tree consist of a SequenceMap object which is a string "recognition sequence" and a vector<string> that holds "enzymes" associated with a sequence.

An example of a line from the rebase210.txt file is:
AarI/CACCTGCNNNN'NNNN/'NNNNNNNNGCAGGTG//
Where the string before the first slash is an enzyme acronym and the rest of the line are recognition sequences separated by a slash

Date of creation: February 25nd, 2019
Date of completition: March 9th, 2019

Purpose: To get comfortable with the properties of AVL trees and its implementations
*/

#include "avl_tree.h"
#include "SequenceMap.h"

#include <iostream>
#include <fstream>
#include <cctype>
#include <sstream>
#include <cstdlib>
#include <string>

// Return false if we have reached two slashes back to back
bool GetNextRecognitionSequence(const std::string &line_of_file, std::string &a_reco_seq, std::size_t &first_slash_pos) {
  // finds the position of the next slash after the first slash
  std::size_t pos_of_next_slash = line_of_file.find("/", first_slash_pos+1);
  // this length is needed since substr needs the number of characters to include in a new string
  int length_of_seq_to_parse = pos_of_next_slash - first_slash_pos;
  //  return false if there are two slashes next to each other
  if (pos_of_next_slash - first_slash_pos == 1) {
    return false;
  } else {
      // using the position of the slahses, we make the recognition sequence equal to the sequence in the line of the file
      a_reco_seq = line_of_file.substr(first_slash_pos+1, length_of_seq_to_parse-1);
      // update slash for next call
      first_slash_pos = pos_of_next_slash;
      return true;
    }
}

// gets the enzyme (i.e., the string before the first slash)
std::string GetEnzymeAcronym(const std::string &line_of_file, const std::size_t &first_slash_pos) {
  return (line_of_file.substr(0, first_slash_pos)); // return the enzyme
}


// @db_filename: an input filename.
// @a_tree: an input tree of the type TreeType. It is assumed to be
// empty.
template <typename TreeType>
void QueryTree(const std::string &db_filename, TreeType &a_tree) {
  std::ifstream file;
  file.open(db_filename);
  if (file.fail()) {
    cerr << "Error -- Cannot read from file. Exit\n";
    exit(1);
  }
  std::string lines_of_text, header_to_ignore, an_enz_acro;
  // Loops 10 times to ignore the header in the file.
  for (int i = 0; i < 10; i++) {
    std::getline(file, header_to_ignore);
  }
  while(std::getline(file, lines_of_text)) {
    if (isalnum(lines_of_text[0])) { // making sure the line is valid.
      std::size_t first_slash_pos = lines_of_text.find('/'); // finds the first slash
      an_enz_acro = GetEnzymeAcronym(lines_of_text, first_slash_pos);
      std::string a_reco_seq;
      while (GetNextRecognitionSequence(lines_of_text, a_reco_seq, first_slash_pos)) {
        SequenceMap new_sequence_map(a_reco_seq, an_enz_acro); // make an object SequenceMap with each sequence from the file
        a_tree.insert(new_sequence_map); // Nodes in the tree are of type SequenceMap.
      }
    }
  }
  // Limited to THREE sequences as per instructions from the homework assignment
  std::cout << "Enter THREE sequences and I will output their associated enzyme acronyms\n";
  std::string user_rec_seque1, user_rec_seque2, user_rec_seque3;
  cin >> user_rec_seque1;
  cin >> user_rec_seque2;
  cin >> user_rec_seque3;
  SequenceMap map1(user_rec_seque1, " ");
  SequenceMap map2(user_rec_seque2, " ");
  SequenceMap map3(user_rec_seque3, " ");
  if (a_tree.contains(map1) || a_tree.contains(map2) || a_tree.contains(map3)) {
    a_tree.PrintEnzymesForUserSequence(map1); std::cout << "\n";
    a_tree.PrintEnzymesForUserSequence(map2); std::cout << "\n";
    a_tree.PrintEnzymesForUserSequence(map3); std::cout << "\n";
  } else {
      std::cout << "The Recognition Sequences entered aren't in this tree\n";
  }
}

/*
Main function given by Professor
*/
int main(int argc, char **argv) {
  std::cout << argv[0] << " second: " <<  argv[1] << " <databasefilename>" << endl;
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <databasefilename>" << endl;
    return 0;
  }
  const std::string db_filename(argv[1]);
  std::cout << "Input filename is " << db_filename << endl;
  std::cout << "I will parse this file and create an AVL tree." << endl;
  AvlTree<SequenceMap> a_tree;
  QueryTree(db_filename, a_tree);
  return 0;
}
