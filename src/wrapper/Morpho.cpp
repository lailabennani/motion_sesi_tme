// lahkim leon
#include "motion/wrapper/Morpho.hpp"

Morpho::Morpho(const int i0, const int i1, const int j0, const int j1)
: spu::module::Stateful(), i0(i0), i1(i1), j0(j0), j1(j1)
{
	const std::string name = "morpho";
	this->set_name(name);
	this->set_short_name(name);
	this->morpho_data = morpho_alloc_data(i0, i1, j0, j1);
	morpho_init_data(this->morpho_data); // check this
	this->tmp_img = ui8matrix(i0, i1, j0, j1);

	auto &t = this->create_task("compute");
	auto si_img = this->template create_2d_socket_in<uint8_t>(t, "in_img", (i1 - i0) + 1, (j1 - j0) + 1); // grayscale
	auto so_img = this->template create_2d_socket_out<uint8_t>(t, "out_img", (i1 - i0) + 1, (j1 - j0) + 1); // grayscale
	
	this->create_codelet(t,
		[si_img, so_img]
			(spu::module::Module &m, spu::runtime::Task &p, const size_t frame_id) -> int {
		auto &morph = static_cast<Morpho&>(m);

		const uint8_t **in_img = p[si_img].get_2d_dataptr<const uint8_t>();
		zero_ui8matrix(morph.tmp_img, morph.i0, morph.i1, morph.j0, morph.j1);
		uint8_t **out_img = p[so_img].get_2d_dataptr<uint8_t>();

		morpho_compute_opening3(morph.morpho_data, in_img, morph.tmp_img, morph.i0, morph.i1, morph.j0, morph.j1);
		morpho_compute_closing3(morph.morpho_data, (const uint8_t **)morph.tmp_img, out_img, morph.i0, morph.i1, morph.j0, morph.j1);

		return spu::runtime::status_t::SUCCESS;
	});
}

Morpho::~Morpho() {
	morpho_free_data(this->morpho_data);
	free_ui8matrix(this->tmp_img, this->i0, this->i1, this->j0, this->j1);
}


