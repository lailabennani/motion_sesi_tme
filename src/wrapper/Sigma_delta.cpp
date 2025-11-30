#include "motion/wrapper/Sigma_delta.hpp"

Sigma_delta::Sigma_delta(const uint8_t** initial_img, const int i0, const int i1, const int j0, const int j1, const uint8_t vmin, const uint8_t vmax, const int n) : spu::module::Stateful(), i0(i0), i1(i1), j0(j0), j1(j1), vmin(vmin), vmax(vmax), n(n){
	
	const std::string name = "sigma_delta";
	this->set_name(name);
	this->set_short_name(name);
	this->sd_data = sigma_delta_alloc_data(i0, i1, j0, j1, vmin, vmax);
	sigma_delta_init_data(this->sd_data, initial_img, this->i0, this->i1, this->j0, this->j1); // laila said check this

	auto &t = this->create_task("compute");
	auto si_img = this->template create_2d_socket_in<uint8_t>(t, "in_img", (i1-i0)+1, (j1-j0)+1); //marcosaidgrayscale
	auto so_img = this->template create_2d_socket_out<uint8_t>(t, "out_img", (i1-i0)+1, (j1-j0)+1); //marcosaidgrayscale
	
	this->create_codelet(t,
		[si_img, so_img]
			(spu::module::Module &m, spu::runtime::Task &p, const size_t frame_id) -> int {
		auto &sd = static_cast<Sigma_delta&>(m);

		const uint8_t **in_img = p[si_img].get_2d_dataptr<const uint8_t>();
		uint8_t **out_img = p[so_img].get_2d_dataptr<uint8_t>();

		sigma_delta_compute(sd.sd_data, in_img, out_img, sd.i0, sd.i1, sd.j0, sd.j1, sd.n); //VERY IMPORTANT : IDK WHAT N MEANS AND WHAT TO PUT HERE

		return spu::runtime::status_t::SUCCESS;
	});

}

Sigma_delta::~Sigma_delta(){
	sigma_delta_free_data(this->sd_data);	
}
