#include "StateMachine.h"

GameState StateMachine::CurrentState = MAP_MODE;
CombatPhase StateMachine::CurrentCombatPhase = PLAYER_TURN;
int StateMachine::Score = 0;
int StateMachine::Wave = 1;
void StateMachine::ChangeState(GameState newState) {
	CurrentState = newState;
}
void StateMachine::ChangeCombatPhase(CombatPhase newPhase) {
	CurrentCombatPhase = newPhase;
}
