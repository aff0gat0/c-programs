#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  char rank;
} Card;

int card_value(Card c) {
  if (c.rank >= '2' && c.rank <= '9') {
    return c.rank - '0';
  }
  if (c.rank == 'T') return 10;
  if (c.rank == 'J' || c.rank == 'Q' || c.rank == 'K') return 10;
  if (c.rank == 'A') return 1;
  return 0;
}

int evaluate_hand(Card *hand, int num_cards) {
  int total = 0;
  int aces = 0;

  // sum all cards (aces count as 1)
  for (int i = 0; i < num_cards; i++) {
    total += card_value(hand[i]);
    if (hand[i].rank == 'A') {
      aces++;
    }
  }

  // use one ace as 11 if it won't bust
  if (aces > 0 && total + 10 <= 21) {
    total += 10;
  }

  return total;
}

void init_deck(Card *deck) {
  int idx = 0;
  char ranks[] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};

  // four of each rank (i'm not storing suit data)
  for (int suit = 0; suit < 4; suit++) {
    for (int r = 0; r < 13; r++) {
      deck[idx].rank = ranks[r];
      idx++;
    }
  }
}

void shuffle_deck(Card *deck) {
  for (int i = 51; i > 0; i--) {
    int j = rand() % (i + 1);
    Card temp = deck[i];
    deck[i] = deck[j];
    deck[j] = temp;
  }
}

int deck_position = 0;

Card deal_card(Card *deck) {
  return deck[deck_position++];
}

int main() {
  srand(time(NULL));

  Card deck[52];
  Card player_hand[12];
  Card dealer_hand[12];
  int player_count = 0;
  int dealer_count = 0;

  // setup
  init_deck(deck);
  shuffle_deck(deck);
  deck_position = 0;

  // deal initial cards
  player_hand[player_count++] = deal_card(deck);
  dealer_hand[dealer_count++] = deal_card(deck);
  player_hand[player_count++] = deal_card(deck);
  dealer_hand[dealer_count++] = deal_card(deck);

  // show initial hands
  printf("Dealer shows: %c\n", dealer_hand[0].rank);
  printf("Your hand: %c %c (total: %d)\n",
          player_hand[0].rank, player_hand[1].rank,
          evaluate_hand(player_hand, player_count));

  // player's turn
  char choice;
  while (1) {
    printf("Hit or stand? (h/s): ");
    scanf(" %c", &choice);

    if (choice == 's' || choice == 'S') break;

    if (choice == 'h' || choice == 'H') {
      player_hand[player_count++] = deal_card(deck);
      printf("You drew: %c\n", player_hand[player_count-1].rank);

      int total = evaluate_hand(player_hand, player_count);
      printf("Your total: %d\n", total);

      if (total > 21) {
        printf("Bust! You lose.\n");
        return 0;
      }
      if (total == 21) break;
    }
  }

  int player_total = evaluate_hand(player_hand, player_count);

  // dealer's turn
  printf("\nDealer reveals: %c %c\n", dealer_hand[0].rank, dealer_hand[1].rank);
  int dealer_total = evaluate_hand(dealer_hand, dealer_count);
  printf("Dealer total: %d\n", dealer_total);

  while (dealer_total < 17) {
    dealer_hand[dealer_count++] = deal_card(deck);
    printf("Dealer draws: %c\n", dealer_hand[dealer_count-1].rank);
    dealer_total = evaluate_hand(dealer_hand, dealer_count);
    printf("Dealer total: %d\n", dealer_total);
  }

  // determine winner
  if (dealer_total > 21) {
    printf("Dealer busts! You win!\n");
  } else if (player_total > dealer_total) {
    printf("You win!\n");
  } else if (dealer_total > player_total) {
    printf("Dealer wins.\n");
  } else {
    printf("Push (tie).\n");
  }

  return 0;
}
