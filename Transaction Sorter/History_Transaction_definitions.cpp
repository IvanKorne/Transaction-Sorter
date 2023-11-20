#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
Transaction::Transaction(std::string ticker_symbol,  unsigned int day_date,  
        unsigned int month_date,  unsigned year_date, 
        bool buy_sell_trans,  unsigned int number_shares,  
        double trans_amount ){
      symbol = ticker_symbol;
      day = day_date;
      month = month_date; 
      year = year_date;
      shares = number_shares;
      amount = trans_amount;
      trans_id = assigned_trans_id;
      assigned_trans_id++;
      cgl = 0;
      share_balance = 0;
      acb = 0;
      acb_per_share = 0;
      if((buy_sell_trans)){
      trans_type = "Buy";
      }
      else{
        trans_type = "Sell";
      }
      p_next = nullptr;
};


// Destructor
Transaction::~Transaction(){
  p_next = nullptr;
};

// Overloaded < operator.
bool Transaction::operator<( Transaction const &other ){
  if(this->get_year() < other.get_year()){
    return true;
  }
  else if(this->get_year() > other.get_year()){
    return false;
  }
  else{
    if(this->get_month() < other.get_month()){
      return true;
    }
    else if(this->get_month() > other.get_month()){
      return false;
    }
    else{
      if(this->get_day() < other.get_day()){
        return true;
      }
      else if(this->get_day() > other.get_day()){
        return false;
      }
      else{
        if(this->get_trans_id() < other.get_trans_id()){
          return true;
        }
        else{
          return false;
        }
      }
    }
  }

  }

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
History::History(){
  p_head = nullptr;
};

// Destructor
History::~History(){
  while(p_head != nullptr){
    Transaction *old_{p_head};
        p_head = p_head->get_next();
        delete old_;
        old_ = nullptr;
  }
};

// insert(...): Insert transaction into linked list.
void History::insert(Transaction *p_new_trans){
  if(p_head == nullptr){
    p_head=(p_new_trans);
  }
  else{
     Transaction *tmp = p_head;
     while(tmp->get_next() != nullptr){
      tmp = tmp->get_next();
      }
      tmp->set_next(p_new_trans);
  }
}

// read_history(...): Read the transaction history from file.
void History::read_history(){
  ece150::open_file();
  ece150::next_trans_entry();
  p_head =  new Transaction(ece150::get_trans_symbol(),ece150::get_trans_day(),
      ece150::get_trans_month(),ece150::get_trans_year(), ece150::get_trans_type(),
      ece150::get_trans_shares(), ece150::get_trans_amount());
  Transaction *tmp = p_head;
  while(ece150::next_trans_entry()){
      tmp->set_next(new Transaction(ece150::get_trans_symbol(),ece150::get_trans_day(),
      ece150::get_trans_month(),ece150::get_trans_year(), ece150::get_trans_type(),
      ece150::get_trans_shares(), ece150::get_trans_amount()));
      tmp = tmp->get_next();
  }
  ece150::close_file();
};
// print() Print the transaction history.
void History::print(){
  std::cout<<"========== BEGIN TRANSACTION HISTORY ============"<<std::endl;
  Transaction *tmp = p_head;
  while(tmp != nullptr){
    tmp->print();
    tmp = tmp->get_next();
  }
  std::cout<<"========= END TRANSACTION HISTORY ============"<<std::endl;
};

// sort_by_date(): Sort the linked list by trade date.
void History::sort_by_date(){
  bool first{true};
  Transaction *sorted = nullptr;
  Transaction *tmp = p_head;
  Transaction *stmp = sorted;
  while(tmp != nullptr){
    Transaction *p_old = tmp;
    std::cout << tmp->get_trans_id()<<std::endl;
    if(!first){
      while(stmp->get_next() != nullptr){
      if(*tmp < *stmp->get_next()){
        break;
      }   
      stmp = stmp->get_next();
      }
    }
    if(first){
        stmp = p_old;
        sorted = p_old;
        tmp = tmp->get_next();
        stmp->set_next(nullptr);
        first = false;
    }
    else{
      Transaction *after = tmp->get_next();
      Transaction *safter = stmp->get_next();
      stmp->set_next(tmp);
      tmp->set_next(safter);
      stmp = sorted;
      tmp = after;
    }
  }
  p_head = sorted;
};


// update_acb_cgl(): Updates the ACB and CGL values.
void History::update_acb_cgl(){
  Transaction *tmp = p_head;
  double acb = 0.0;
  double share = 0.0;
  double acbshare = 0.0;
  double cgl = 0.0;
  while(tmp != nullptr){
    if(tmp->get_trans_type() == true){
    acb += tmp->get_amount();
    share += tmp->get_shares();
    }
    else if(tmp->get_trans_type() == false){
    acb -= (tmp->get_shares()*acbshare);
    share -= tmp->get_shares();
    }
    acbshare = acb/share;
    tmp->set_acb(acb);
    tmp->set_share_balance(share);
    tmp->set_acb_per_share(acbshare);
    if(tmp->get_trans_type() == false){
    cgl += (tmp->get_amount()) - tmp->get_shares()*tmp->get_acb_per_share();
    tmp->set_cgl(cgl);
    cgl = 0.0;
    }
    tmp = tmp->get_next();
  }
};

// compute_cgl(): )Compute the ACB, and CGL.
double History::compute_cgl(unsigned int year){
  Transaction *tmp = p_head;
  double cgl = 0;
  while(tmp->get_year() != year){
    tmp = tmp->get_next();
  }
  while((tmp != nullptr) && (tmp->get_year() == year)){
    if(tmp->get_trans_type() == false){
     cgl += (tmp->get_amount()) - tmp->get_shares()*tmp->get_acb_per_share();
    }
    tmp = tmp->get_next();

  }
  return cgl;
};


// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
