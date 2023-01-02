// Copyright (c) 2023 UltiMaker
// CuraEngine is released under the terms of the AGPLv3 or higher

#ifndef UTILS_DEBUG_VISUAL_LOGGER_H
#define UTILS_DEBUG_VISUAL_LOGGER_H

#include <mutex>
#include <utility>

#include <vtu11/vtu11.hpp>

#include "Application.h"
#include "utils/concepts/geometry.h"

namespace cura::debug
{
class VisualLogger
{
public:
    VisualLogger() : vtu_dir_{}, data_set_info_{} {};

    explicit VisualLogger(std::filesystem::path vtu_dir) : vtu_dir_{ std::move(vtu_dir) }, data_set_info_{ vtu11::DataSetInfo{ "Property_1", vtu11::DataSetType::PointData, 1 }, { "Property_1", vtu11::DataSetType::CellData, 1 } }
    {
        vtu11::writePVtu(vtu_dir_.string(), "CuraEngine", data_set_info_, 1);
    }

    ~VisualLogger()
    {
        vtu11::writePVtu(vtu_dir_.string(), "CuraEngine", data_set_info_, idx_); // Need to write this again since we now know the exact number of vtu files
    }

    VisualLogger(const VisualLogger& other) : vtu_dir_{ other.vtu_dir_ }, idx_{ other.idx_ }, data_set_info_{ other.data_set_info_ } {};

    VisualLogger(VisualLogger&& other) noexcept : vtu_dir_{ std::exchange(other.vtu_dir_, {}) }, idx_{ std::exchange(other.idx_, 0) }, data_set_info_{ std::exchange(other.data_set_info_, {}) } {};

    VisualLogger& operator=(const VisualLogger& other)
    {
        return *this = VisualLogger(other);
    }

    VisualLogger& operator=(VisualLogger&& other) noexcept
    {
        std::swap(vtu_dir_, other.vtu_dir_);
        std::swap(idx_, other.idx_);
        std::swap(data_set_info_, other.data_set_info_);
        return *this;
    }

    void log(const isMesh auto& mesh)
    {
        // TODO: write mesh to vtu partition5
    }

    void log(const isPolygon auto& poly)
    {
        // TODO: convert to polygon
        std::vector<double> points0{
            0.0, 0.0, 0.5, 0.0, 0.3, 0.5, 0.0, 0.7, 0.5, 0.0, 1.0, 0.5, // 0,  1,  2,  3
            0.5, 0.0, 0.5, 0.5, 0.3, 0.5, 0.5, 0.7, 0.5, 0.5, 1.0, 0.5, // 4,  5,  6,  7
        };
        std::vector<vtu11::VtkIndexType> connectivity0{
            0, 4, 5, 1, // 0
            1, 5, 6, 2, // 1
            2, 6, 7, 3, // 2
        };
        std::vector<vtu11::VtkIndexType> offsets0{ 4, 8, 12 };
        std::vector<vtu11::VtkCellType> types0{ 9, 9, 9 };
        vtu11::Vtu11UnstructuredMesh meshPartition0{ points0, connectivity0, offsets0, types0 };
        std::vector<double> pointData0{ 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 };
        std::vector<double> cellData0{ 3.2, 4.3, 5.4 };
        std::vector<vtu11::DataSetData> dataSetData0{ pointData0, cellData0 };

        std::lock_guard<std::mutex> guard(mutex_);
        vtu11::writePartition(vtu_dir_.string(), "CuraEngine", meshPartition0, data_set_info_, dataSetData0, idx_++, "RawBinary");
    }

private:
    std::mutex mutex_;
    std::filesystem::path vtu_dir_;
    size_t idx_{ 0 };
    std::vector<vtu11::DataSetInfo> data_set_info_;
};

using shared_visual_logger = std::shared_ptr<VisualLogger>;
} // namespace cura::debug


#endif // UTILS_DEBUG_VISUAL_LOGGER_H
