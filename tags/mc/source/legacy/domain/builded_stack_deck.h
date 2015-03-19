
#ifndef PEGAS_FAKESPIDER_BUILDED_STACK_DECK_H
#define PEGAS_FAKESPIDER_BUILDED_STACK_DECK_H
#pragma once

namespace Pegas
{
	
	class BuildedStackDeck: public GameObject
	{
	public:
		typedef std::list<RESOURCEID> BottomStackCardList;

		enum
		{
			k_relativeLeftOffset = 12
		};

		virtual ~BuildedStackDeck() {};
        
		virtual void draw(GrafManager& context);

		void reset();
		void addStack(CardStack* pStack);

		virtual CURCOORD getWidth();
		virtual CURCOORD getHeight();

		int getStackCount() { return m_bottomCards.size(); };
	
	private:
		BottomStackCardList m_bottomCards;
	};
}

#endif //PEGAS_FAKESPIDER_BUILDED_STACK_DECK_H