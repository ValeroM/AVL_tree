/*
Author Marco Valero
Part 1 of Project #2
*/

#ifndef SEQUENCE_MAP_H_
#define SEQUENCE_MAP_H_

#include <iostream>
#include <vector>
#include <string>

class SequenceMap {
public:
  // Big 5 as default
  SequenceMap(const SequenceMap &rhs) = default; // Copy-constructor.
  SequenceMap(SequenceMap &&rhs) = default; // Move-constructor.
  SequenceMap& operator=(const SequenceMap &rhs) = default; // Copy-assignment.
  SequenceMap& operator=(SequenceMap &&rhs) = default; // Move-assignment.
  ~SequenceMap() = default;  // Destructor

  /*
  Two parameter constructor
  @a_rec_seq: a recoginticion sequence to initialize our private variable
  @an_enz_acro: An enzyme acronym to push to our vector
  */
  SequenceMap(const std::string &a_rec_seq, const std::string &an_enz_acro) : recognition_sequence_(a_rec_seq) {
    enzyme_acronyms_.push_back(an_enz_acro);
  }

  /*
  String comparison between the recognition_sequence_ strings
  */
  bool operator<(const SequenceMap &rhs) const {
    return (recognition_sequence_ < rhs.recognition_sequence_);
  }

  /*
  Overload the operator<<
  */
  friend std::ostream& operator<<(std::ostream& os, const SequenceMap& some_seq_map) {
    os << "Recognition sequence: " << some_seq_map.GetRecogSeq() << " Enzyme Acronyms: ";
    for (std::size_t i = 0; i < some_seq_map.enzyme_acronyms_.size(); i++) {
      os << some_seq_map.enzyme_acronyms_[i] << " ";
    }
    os << std::endl;
    return os;
  }

  /*
  Merging enzyme_acronyms_
  */
  void Merge (const SequenceMap &other_sequence) {
    for (std::size_t i = 0; i < other_sequence.enzyme_acronyms_.size(); i++) {
      enzyme_acronyms_.push_back(other_sequence.enzyme_acronyms_[i]);
    }
  }

  /*
  Gets the sequece
  */
  std::string GetRecogSeq() const {
    return recognition_sequence_;
  }

  /*
  Prints each enzyme acronym separated by a space
  */
  void PrintVectorOfEnzymes() {
    for (std::size_t i = 0; i < enzyme_acronyms_.size(); i++) {
      std::cout << enzyme_acronyms_[i] << " ";
    }
  }
private:
  std::string recognition_sequence_;
  std::vector<std::string> enzyme_acronyms_;
};

#endif /* SEQUENCE_MAP_H_ */
