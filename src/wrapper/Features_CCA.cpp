// lahkim leon

#include "motion/wrapper/Features_CCA.hpp"


Features_CCA::Features_CCA(const int i0, const int i1, const int j0, const int j1, const size_t max_sz)
: spu::module::Stateful(), i0(i0), i1(i1), j0(j0), j1(j1), max_sz(max_sz)
{
    const std::string name = "features_cca";
    this->set_name(name);
    this->set_short_name(name);

    auto &t = this->create_task("extract");

    // we can use a fwd socket here
    auto si_labels = this->template create_2d_socket_in<uint32_t>(t, "in_labels", (i1 - i0) + 1, (j1 - j0) + 1); // fwd
    auto si_n_RoIs = this->template create_socket_in<uint32_t>(t, "in_n_RoIs", 1); // fwd
    //auto so_labels = this->template create_2d_socket_out<uint32_t>(t, "out_labels", (i1 - i0) + 1, (j1 - j0) + 1); //fwd
    auto so_RoIs   = this->template create_socket_out<uint8_t>(t, "out_RoIs", max_sz * sizeof(RoI_t));
    auto so_n_RoIs = this->template create_socket_out<uint32_t>(t, "out_n_RoIs", 1); // fwd

    this->create_codelet(t,
        [si_labels, si_n_RoIs, /*so_labels,*/ so_RoIs, so_n_RoIs]
            (spu::module::Module &m, spu::runtime::Task &p, const size_t frame_id) -> int {
        auto &cca = static_cast<Features_CCA&>(m);

        const uint32_t **in_labels = p[si_labels].get_2d_dataptr<const uint32_t>();
        auto in_n_RoIs = p[si_n_RoIs].get_dataptr<const uint32_t>()[0]; // not sure. verify
        //uint32_t **out_labels = p[so_labels].get_2d_dataptr<uint32_t>();
        RoI_t *out_RoIs = p[so_RoIs].get_dataptr<RoI_t>(); // verify
        uint32_t *out_n_RoIs = p[so_n_RoIs].get_dataptr<uint32_t>();

        features_extract(in_labels, cca.i0, cca.i1, cca.j0, cca.j1, out_RoIs, in_n_RoIs);

        *out_n_RoIs = in_n_RoIs;

        //for (int i = cca.i0; i <= cca.i1; i++)
        //    memset(out_labels[i], 0, (cca.j1 - cca.j0 + 1) * sizeof(uint32_t)); // verify

        return spu::runtime::status_t::SUCCESS;
    });

}