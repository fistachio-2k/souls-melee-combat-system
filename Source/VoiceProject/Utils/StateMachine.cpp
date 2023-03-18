#include "StateMachine.h"

template <typename E, typename T0>
void StateMachine<E, T0>::SetState(E State)
{
	if (State != CurrentState)
	{
		CurrentState = State;
		if (StateHandlers.Contains(State))
		{
			StateHandlers[State].OnStateBegin.Execute();
		}
	}
}

template <typename E, typename T0>
template <class UserClass>
void StateMachine<E, T0>::RegisterStateHandler(E State, UserClass* InObject, void (UserClass::*OnBegin)())
{
	FStateHandler NewStateHandler = {State};
	NewStateHandler.OnStateBegin.BindUObject(InObject, OnBegin);
	StateHandlers.Add(State, NewStateHandler);
}

template <typename E, typename T0>
void StateMachine<E, T0>::RemoveStateHandler(E State)
{
	StateHandlers.Remove(State);
}

