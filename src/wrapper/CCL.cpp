#include "motion/wrapper/CCL.hpp"

CCL::CCL(const int i0, const int i1, const int j0, const int j1, const int p_cca_roi_max1)
: spu::module::Stateful(), i0(i0), i1(i1), j0(j0), j1(j1), p_cca_roi_max1(p_cca_roi_max1)
{
	const std::string name = "CCL";
	this->set_name(name);
	this->set_short_name(name);
	this->CCL_data = CCL_LSL_alloc_data(i0, i1, j0, j1);
	CCL_LSL_init_data(this->CCL_data);

	auto &t = this->create_task("apply");
	auto si_img = this->template create_2d_socket_in<uint8_t>(t, "in_img", (i1-i0)+1, (j1-j0)+1);
	auto so_labels = this->template create_2d_socket_out<uint32_t>(t, "out_labels", (i1-i0)+1, (j1-j0)+1);
	auto so_n_RoIs = this->template create_socket_out<uint32_t>(t, "out_n_RoIs_tmp0", 1);
	this->create_codelet(t,
		[si_img, so_labels, so_n_RoIs]
			(spu::module::Module &m, spu::runtime::Task &p, const size_t frame_id) -> int{
		auto &ccl = static_cast<CCL&>(m);

		const uint8_t ** in_img = p[si_img].get_2d_dataptr<const uint8_t>();
		uint32_t ** label_img = p[so_labels].get_2d_dataptr<uint32_t>(); 			
		uint32_t * n_RoIs_tmp0 = p[so_n_RoIs].get_dataptr<uint32_t>();

		*n_RoIs_tmp0 = CCL_LSL_apply(ccl.CCL_data, in_img, label_img, 0); //idk if the 0 is right
		
		assert(*n_RoIs_tmp0 <= (uint32_t)ccl.p_cca_roi_max1);
	
		return spu::runtime::status_t::SUCCESS;		
			
	});
}

CCL::~CCL(){
	CCL_LSL_free_data(this->CCL_data);
}

