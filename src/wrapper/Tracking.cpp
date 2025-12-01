#include "motion/wrapper/Tracking.hpp"

Tracking::Tracking(const size_t r_extrapol, const size_t fra_obj_min, const uint8_t save_RoIs_id, const uint8_t extrapol_order_max, const float min_extrapol_ratio_S, const size_t max_history_size, const size_t max_RoIs_size)
: spu::module::Stateful(), r_extrapol(r_extrapol), fra_obj_min(fra_obj_min), save_RoIs_id(save_RoIs_id), extrapol_order_max(extrapol_order_max), min_extrapol_ratio_S(min_extrapol_ratio_S), max_history_size(max_history_size), max_RoIs_size(max_RoIs_size)
{
	const std::string name = "Tracking";
	this->set_name(name);
	this->set_short_name(name);

	this->tracking_data = tracking_alloc_data(max_history_size, max_RoIs_size);
	tracking_init_data(this->tracking_data);

	auto &t = this->create_task("perform");

	auto si_RoIs = this->template create_socket_in<uint8_t>(t, "in_RoIs", max_RoIs_size * sizeof(RoI_t));
	auto si_n_RoIs = this->template create_socket_in<uint32_t>(t, "in_n_RoIs", 1);

	this->create_codelet(t,
		[si_RoIs, si_n_RoIs]
		(spu::module::Module &m, spu::runtime::Task &p, const size_t frame_id) -> int {
			auto &track = static_cast<Tracking&>(m);

			const RoI_t* in_RoIs = p[si_RoIs].get_dataptr<const RoI_t>();
			const uint32_t in_n_RoIs = p[si_n_RoIs].get_dataptr<const uint32_t>()[0];
			
			tracking_perform(track.tracking_data, 
				in_RoIs, 
				in_n_RoIs, 
				frame_id, 
				track.r_extrapol, 
				track.fra_obj_min, 
				track.save_RoIs_id, 
				track.extrapol_order_max, 
				track.min_extrapol_ratio_S
			);

			return spu::runtime::status_t::SUCCESS;		
	});

}

Tracking::~Tracking(){
	tracking_free_data(this->tracking_data);
}

tracking_data_t *Tracking::get_tracking_data() const {
    return this->tracking_data;
}