//
// Created by Niels on 6/19/2019.
//

#ifndef IPASS_LINK_STATE_ROUTER_HPP
#define IPASS_LINK_STATE_ROUTER_HPP

#include "../router.hpp"
#include "../../link_state/calculator.hpp"

using link_state::calculator;
using link_state::node;
namespace mesh { //Todo: Pagination for shitloads of nodes
    namespace routers {
        class link_state : public router {
            bool is_updated = false;
            calculator<node_id, uint8_t, 5, 10> ls_calc;


            void graph_update_other(const node_id &other, const message &message);

            void fill_update_message(message &message);

        public:
            explicit link_state(connectivity_adapter &connectivity);

            void update_neighbours() override;

            void send_update() override;


            void initial_update() override;

            void on_routing_message(message &message) override;

            node_id get_next_hop(const node_id &receiver) override;

            calculator<node_id, uint8_t, 5, 10> &get_calculator();

        };
    }
}

#endif //IPASS_LINK_STATE_ROUTER_HPP
