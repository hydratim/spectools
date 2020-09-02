/* Spectrum tools raw wrapper
 *
 * Simple child process wrapper that parses the output and stores it in a fixed size FIFO window.
 *
 * Tim Armstrong/hydratim <tim@plaintextnerds.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Thanks to Mike Kershaw/Dragorn <dragorn@kismetwireless.net> who wrote the original Spectools.
 */
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include "rapidjson/document.h"
#include "spectool_wrapper.h"

using namespace spectools_ng;

//Scan

Scan::Scan() {
    device_name = "";
    freq_lower=0;
    freq_upper=0;
    sample_res=0;
    number_of_samples=0;
}

Scan::Scan(const std::string& line) {
    //  {"device": device_name, "freq_range": [low, high], "sample_res": sample_res, "samples": n_samples, "values": *values}

    rapidjson::Document doc;
    doc.Parse(line.c_str());
    device_name = doc["device"].GetString();
    freq_lower = doc["freq_range"][0].GetUint();
    freq_upper = doc["freq_range"][1].GetUint();
    sample_res = doc["sample_res"].GetUint();
    number_of_samples = doc["samples"].GetUint();
    samples.reserve(number_of_samples);
    for (auto& v : doc["values"].GetArray()) {
        samples.emplace_back(v.GetInt());
    }
}

std::string Scan::getDeviceName() const {
    return device_name;
}

unsigned int Scan::getFreqLower() const {
    return freq_lower;
}

unsigned int Scan::getFreqUpper() const {
    return freq_upper;
}

unsigned int Scan::getSampleRes() const {
    return sample_res;
}

unsigned int Scan::getNumberOfSamples() const {
    return number_of_samples;
}

std::vector<int>::const_iterator Scan::cbegin() {
    return samples.cbegin();
}

std::vector<int>::const_iterator Scan::cend() {
    return samples.cend();
}

std::vector<int>::iterator Scan::begin() {
    return samples.begin();
}

std::vector<int>::iterator Scan::end() {
    return samples.end();
}

std::size_t Scan::size() {
    return number_of_samples;
}

std::ostream& spectools_ng::operator<<(std::ostream &os, const Scan &scan) {
    os << "{\"device_name\": " << scan.device_name << ", \"freq_range\": [" << scan.freq_lower << ", "
       << scan.freq_upper << "], \"sample_res\": " << scan.sample_res << ", \"number_of_samples\": " << scan.number_of_samples << "}" << std::endl;
    return os;
}

// ScanBuffer

ScanBuffer::ScanBuffer(unsigned int maxLength): max_length(maxLength) {}

ScanBuffer::ScanBuffer() {
    max_length = 0;
}

unsigned int ScanBuffer::getMaxLength() const {
    return max_length;
}

void ScanBuffer::resize(unsigned int maxLength) {
    max_length = maxLength;
    while (window.size() > max_length) {
        window.pop_front();
    }
}

unsigned int ScanBuffer::size() {
    return window.size();
}

scan_vector ScanBuffer::getBufferView() {
    scan_vector vect;
    std::copy(window.begin(), window.end(), std::back_inserter(vect));
    return vect;
}

void ScanBuffer::push_back(std::shared_ptr<Scan> scan) {
    window.push_back(scan);
    while (window.size() > max_length) {
        window.pop_front();
    }
}

// SpecToolsRaw

void reader(bp::ipstream *source, std::shared_ptr<std::vector<callback>> device_callbacks, std::shared_ptr<string_vector> _keys, std::shared_ptr<scan_storage_map> _buffer, unsigned int &max_buffer) {
    std::cout << "Reader thread initialising..." << std::endl;
    std::string line;
    std::shared_ptr<Scan> scan;
    std::string device_name;
    auto result = _keys->begin();
    std::getline(*source, line);
    while (std::getline(*source, line)) {
        scan = std::make_shared<Scan>(line);
        device_name = scan->getDeviceName();
        if (!(_keys->empty())) {
            result = std::find(_keys->begin(), _keys->end(), device_name);
        }
        else {
            result = _keys->end();
        }
        if (result != _keys->end()) {
        } else {
            _keys->push_back(device_name);
            (*_buffer)[device_name] = ScanBuffer(max_buffer);
            for (auto callback: *device_callbacks){
                callback(device_name);
            }
        }
        (*_buffer)[device_name].push_back(scan);
    }
}

unsigned int SpectoolsRaw::getMaxLength() const {
    return buffer_depth;
}

SpectoolsRaw::SpectoolsRaw() {
    std::cout << "Launching spectool_raw..." << std::endl;
    child = bp::child("./spectool_raw", "-j", bp::std_out > child_stream);
    std::string line;
    rapidjson::Document doc;
    if (child.running()) {
        std::cout << "Attempting to stream" << std::endl;
        while (true) {
            std::getline(child_stream, line);
            std::cout << line << std::endl;
            rapidjson::ParseResult ok = doc.Parse(line.c_str());
            if (ok) {
                break;
            }
        }
    }
    std::cout << "Stream ready launching reader" << std::endl;
    keys = std::make_shared<string_vector>();
    buffer = std::make_shared<scan_storage_map>();
    device_callbacks = std::make_shared<std::vector<callback>>();
    // launch reader thread
    reader_thread = std::thread(reader, &child_stream, device_callbacks, keys, buffer, std::ref(buffer_depth));
}

const std::vector<std::string> &SpectoolsRaw::getKeys() const {
    return *keys;
}

ScanBuffer &SpectoolsRaw::getBuffer(std::string &key) {
    return buffer->at(key);
}

int SpectoolsRaw::join() {
    child.join();
    reader_thread.join();
    return child.exit_code();
}

void SpectoolsRaw::attach_listener(callback listener) {
    device_callbacks->push_back(listener);
}


