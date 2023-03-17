#include "StateMachine.h"

#include "Microsoft/AllowMicrosoftPlatformTypes.h"

template <typename E, typename T0>
void StateMachine<E, T0>::SetState(E NewState)
{
	if (NewState != CurrentState)
	{
		CurrentState = NewState;
	}

	// StateMachine<EAssociativity> test = new StateMachine();
}

template <typename E, typename T0>
void StateMachine<E, T0>::StateBegins()
{
	OnStateBegin.Broadcast(CurrentState);
}

template <typename E, typename T0>
void StateMachine<E, T0>::StateEnd()
{
	OnStateEnd.Broadcast(CurrentState);
}

