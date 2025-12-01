#include "motion/wrapper/KNN.hpp"

KNN::KNN(const size_t max_sz, const int k, const uint32_t max_dist, const float min_ratio_S) {
        const std::string name = "knn";
        this->set_name(name);
        this->set_short_name(name);

        this->knn_data = kNN_alloc_data(max_sz);
        kNN_init_data(this->knn_data);
        auto &t = this->create_task("match");
        
        // create sockets
        auto si_RoIs0 = this->template create_socket_in<uint8_t>(t, "in_RoIs0", max_sz * sizeof(RoI_t));
        auto si_RoIs1 = this->template create_socket_in<uint8_t>(t, "in_RoIs1", max_sz * sizeof(RoI_t));
        auto si_n_RoIs1 = this->template create_socket_in<uint32_t>(t, "in_n_RoIs1", 1);
        auto si_n_RoIs0 = this->template create_socket_in<uint32_t>(t, "in_n_RoIs0", 1);

        auto so_RoIs0 = this->template create_socket_out<uint8_t>(t, "out_RoIs0", max_sz * sizeof(RoI_t));
        auto so_n_RoIs0 = this->template create_socket_out<uint32_t>(t, "out_n_RoIs0", 1);
        auto so_RoIs1 = this->template create_socket_out<uint8_t>(t, "out_RoIs1", max_sz * sizeof(RoI_t));
        auto so_n_RoIs1 = this->template create_socket_out<uint32_t>(t, "out_n_RoIs1", 1);

        this->create_codelet(t,
            [si_RoIs0, si_RoIs1, si_n_RoIs0, si_n_RoIs1, so_RoIs0, so_n_RoIs0, so_RoIs1, so_n_RoIs1, k, max_dist, min_ratio_S]
                (spu::module::Module &m, spu::runtime::Task &p, const size_t frame_id) -> int {
            auto &knn = static_cast<KNN&>(m);

            const RoI_t *in_RoIs0 = p[si_RoIs0].get_dataptr<const RoI_t>();
            const RoI_t *in_RoIs1 = p[si_RoIs1].get_dataptr<const RoI_t>();
            uint32_t in_n_RoIs0  = p[si_n_RoIs0].get_dataptr<const uint32_t>()[0];
            uint32_t in_n_RoIs1  = p[si_n_RoIs1].get_dataptr<const uint32_t>()[0];

            RoI_t *out_RoIs0 = p[so_RoIs0].get_dataptr<RoI_t>();
            RoI_t *out_RoIs1 = p[so_RoIs1].get_dataptr<RoI_t>();
            uint32_t *out_n_RoIs0 = p[so_n_RoIs0].get_dataptr<uint32_t>();
            uint32_t *out_n_RoIs1 = p[so_n_RoIs1].get_dataptr<uint32_t>();

            memcpy(out_RoIs0, in_RoIs0, in_n_RoIs0 * sizeof(RoI_t));
            memcpy(out_RoIs1, in_RoIs1, in_n_RoIs1 * sizeof(RoI_t));

            kNN_match(knn.knn_data,
                      out_RoIs0, in_n_RoIs0,
                      out_RoIs1, in_n_RoIs1,
                      k, max_dist, min_ratio_S);

            *out_n_RoIs0 = in_n_RoIs0;
            *out_n_RoIs1 = in_n_RoIs1;

            return spu::runtime::status_t::SUCCESS;
        });
}

KNN::~KNN() {
    kNN_free_data(this->knn_data);
}


kNN_data_t *KNN::get_kNN_data() const { return this->knn_data; }