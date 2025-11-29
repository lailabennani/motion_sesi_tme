#include "motion/wrapper/KNN.hpp"

KNN::KNN(const size_t knn_max_sz, const size_t RoIs_max_sz, const int k, const uint32_t max_dist, const float min_ratio_S) {
        const std::string name = "knn";
        this->set_name(name);
        this->set_short_name(name);

        this->knn_data = kNN_alloc_data(knn_max_sz);
        kNN_init_data(this->knn_data);
        auto &t = this->create_task("match");
        
        // create sockets
        auto si_RoIs0 = this->template create_socket_in<uint8_t>(t, "in_RoIs0", RoIs_max_sz * sizeof(RoI_t));
        auto si_RoIs1 = this->template create_socket_in<uint8_t>(t, "in_RoIs1", RoIs_max_sz * sizeof(RoI_t));
        auto si_n_RoIs1 = this->template create_socket_in<uint32_t>(t, "in_n_RoIs1", 1);
        auto si_n_RoIs0 = this->template create_socket_in<uint32_t>(t, "in_n_RoIs0", 1);

        auto so_RoIs1 = this->template create_socket_out<uint8_t>(t, "out_RoIs1", RoIs_max_sz * sizeof(RoI_t));
        auto so_n_RoIs0 = this->template create_socket_out<uint32_t>(t, "out_n_RoIs0", 1);

        this->create_codelet(t,
            [si_RoIs0, si_RoIs1, si_n_RoIs0, si_n_RoIs1, so_RoIs1, so_n_RoIs0, k, max_dist, min_ratio_S]
                (spu::module::Module &m, spu::runtime::Task &p, const size_t frame_id) -> int {
            auto &knn = static_cast<KNN&>(m);

            RoI_t *in_RoIs0 = p[si_RoIs0].get_dataptr<uint8_t>();
            RoI_t *in_RoIs1 = p[si_RoIs1].get_dataptr<uint8_t>();
            uint32_t n_RoIs0  = p[si_n_RoIs0].get_dataptr<const uint32_t>()[0];
            uint32_t n_RoIs1  = p[si_n_RoIs1].get_dataptr<const uint32_t>()[0];

            RoI_t *out_RoIs1 = p[so_RoIs1].get_dataptr<uint8_t>();
            uint32_t *out_n_RoIs0 = p[so_n_RoIs0].get_dataptr<uint32_t>();

            kNN_match(knn.knn_data,
                      in_RoIs0, n_RoIs0,
                      in_RoIs1, n_RoIs1,
                      k, max_dist, min_ratio_S);

            memcpy(out_RoIs1, in_RoIs1, n_RoIs1 * sizeof(RoI_t));
            *out_n_RoIs0 = n_RoIs0;

            return spu::runtime::status_t::SUCCESS;
        });
}

KNN::~KNN() {
    kNN_free_data(this->knn_data);
}