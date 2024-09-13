using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FSM_ANT_II
{
    public class FSM
    {
        private Action activeState; 
 
        public FSM()
        {

        }

        public Action setState(Action state)
        {
            activeState = state;
            return state;
        }

        public void update()
        {
            activeState?.Invoke();
        }
      
    }
}
