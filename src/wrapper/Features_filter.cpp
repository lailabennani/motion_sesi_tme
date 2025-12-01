#include "motion/wrapper/Features_filter.hpp"

Features_filter::Features_filter(const int i0, const int i1, const int j0, const int j1, const size_t max_RoIs_size, const int p_flt_s_min, const int p_flt_s_max, const int p_cca_roi_max2)
: spu::module::Stateful(), i0(i0), i1(i1), j0(j0), j1(j1), max_RoIs_size(max_RoIs_size), p_flt_s_min(p_flt_s_min), p_flt_s_max(p_flt_s_max), p_cca_roi_max2(p_cca_roi_max2)
{
	const std::string name = "Features_filter";
	this->set_name(name);
	this->set_short_name(name);
		
	auto &t = this->create_task("filter");

	this->RoIs_tmp = features_alloc_RoIs(this->max_RoIs_size);
	features_init_RoIs(this->RoIs_tmp, this->max_RoIs_size);

        // we can use a fwd socket here
    	auto si_labels = this->template create_2d_socket_in<uint32_t>(t, "in_labels", (i1 - i0) + 1, (j1 - j0) + 1); // fwd
    	auto si_RoIs   = this->template create_socket_in<uint8_t>(t, "in_RoIs", max_RoIs_size * sizeof(RoI_t));
	auto si_n_RoIs = this->template create_socket_out<uint32_t>(t, "in_n_RoIs", 1); // fwd	
    	auto so_labels = this->template create_2d_socket_out<uint32_t>(t, "out_labels", (i1 - i0) + 1, (j1 - j0) + 1); //fwd
    	auto so_RoIs   = this->template create_socket_out<uint8_t>(t, "out_RoIs", max_RoIs_size * sizeof(RoI_t));
    	auto so_n_RoIs = this->template create_socket_out<uint32_t>(t, "out_n_RoIs", 1); // fwd	
	
	this->create_codelet(t,
		[si_labels, si_RoIs, si_n_RoIs, so_labels, so_RoIs, so_n_RoIs]
		(spu::module::Module &m, spu::runtime::Task &p, const size_t frame_id) -> int {
		auto &filter = static_cast<Features_filter&>(m);

		const uint32_t ** in_labels = p[si_labels].get_2d_dataptr<const uint32_t>();
		const RoI_t * in_RoIs = p[si_RoIs].get_dataptr<const RoI_t>();
		const uint32_t * in_n_RoIs = p[si_n_RoIs].get_dataptr<const uint32_t>();

		uint32_t ** out_labels = p[so_labels].get_2d_dataptr<uint32_t>();
		RoI_t * out_RoIs = p[so_RoIs].get_dataptr<RoI_t>();
		uint32_t * out_n_RoIs = p[so_n_RoIs].get_dataptr<uint32_t>();

		memcpy(filter.RoIs_tmp, in_RoIs, (*in_n_RoIs) * sizeof(RoI_t));

		*out_n_RoIs = features_filter_surface(in_labels, out_labels, filter.i0, filter.i1, filter.j0, filter.j1, filter.RoIs_tmp, *in_n_RoIs, filter.p_flt_s_min, filter.p_flt_s_max);

		assert(*out_n_RoIs <= (uint32_t)filter.p_cca_roi_max2);
		
		features_shrink_basic(filter.RoIs_tmp, *in_n_RoIs, out_RoIs);
		return spu::runtime::status_t::SUCCESS;
	});
}

Features_filter::~Features_filter() {
	features_free_RoIs(this->RoIs_tmp);
}
