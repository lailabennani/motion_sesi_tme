#include "motion/wrapper/Features_filter.hpp"

Features_filter::Features_filter(const int i0, const int i1, const int j0, const int j1, const size_t max_RoIs_size)
: spu::module::Stateful(), i0(i0), i1(i1), j0(j0), j1(j1), max_RoIs_size(max_RoIs_size)
{
	const std::string name = "Features_filter";
	this->set_name(name);
	this->set_short_name(name);
	
	this->RoIs_local = features_alloc_RoIs(this->max_RoIs_size);
	features_init_RoIs(this->RoIs_local, this->max_RoIs_size);
	
	auto &t = this->create_task("filter");

        // we can use a fwd socket here
    	auto si_labels = this->template create_2d_socket_in<uint32_t>(t, "in_labels", (i1 - i0) + 1, (j1 - j0) + 1); // fwd
    	auto si_RoIs   = this->template create_socket_in<uint8_t>(t, "in_RoIs", max_RoIs_size * sizeof(RoI_t));
    	auto so_labels = this->template create_2d_socket_out<uint32_t>(t, "out_labels", (i1 - i0) + 1, (j1 - j0) + 1); //fwd
    	auto so_RoIs   = this->template create_socket_out<uint8_t>(t, "out_RoIs", max_RoIs_size * sizeof(RoI_t));
    	auto so_n_RoIs = this->template create_socket_out<uint32_t>(t, "out_n_RoIs", 1); // fwd	
	
	this->create_codelet(t,
		[si_labels, si_RoIs, so_labels, so_n_RoIs, so_RoIs]
		(spu::module::Module &m, spu::runtime::Task &p, const size_t frame_id) -> int {
		auto &filter = static_cast<Features_filter&>(m);

		const uint32_t ** in_labels = p[si_labels].get_2d_dataptr<const uint32_t>();
		RoI_t * in_RoIs = p[si_RoIs].get_dataptr<RoI_t>();

		uint32_t ** out_labels = p[so_labels].get_2d_dataptr<uint32_t>();
		RoI_t * out_RoIs = p[so_RoIs].get_dataptr<RoI_t>();

		features_filter_surface(in_labels, out_labels, filter.i0, filter.i1, filter.j0, filter.j1, in_RoIs, filter.max_RoIs_size, 50, 100000);

		features_shrink_basic(in_RoIs, filter.max_RoIs_size, out_RoIs);
		
		return spu::runtime::status_t::SUCCESS;
	});
}


