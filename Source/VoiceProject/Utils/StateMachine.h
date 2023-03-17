#pragma once

#include <type_traits>

template <typename E, typename = std::enable_if_t<std::is_enum_v<E>>>
class StateMachine
{
	DECLARE_MULTICAST_DELEGATE_OneParam(StateModifyDelegate, E);

	E CurrentState;
	
public:
	StateMachine() = default;
	explicit StateMachine(E CurrentState): CurrentState(CurrentState) {}
	
	E GetCurrentState() const { return CurrentState; }
	void SetState(E NewState);

	StateModifyDelegate OnStateBegin;
	StateModifyDelegate OnStateEnd;

private:
	void StateBegins();
	void StateEnd();
};
