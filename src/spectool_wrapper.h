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
#ifndef SPECTOOLS_NG_SPECTOOL_WRAPPER_H
#define SPECTOOLS_NG_SPECTOOL_WRAPPER_H

#include <boost/process.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/read_until.hpp>
#include <ostream>

namespace bp = boost::process;
namespace asio = boost::asio;

namespace spectools_ng {

    class Scan {
        std::string device_name;
        unsigned int freq_lower;
        unsigned int freq_upper;
        unsigned int sample_res;
        unsigned int number_of_samples;
        std::vector<int> samples;
    public:
        Scan();
        explicit Scan(const std::string &line);
        [[nodiscard]] std::string getDeviceName() const;
        [[nodiscard]] unsigned int getFreqLower() const;
        [[nodiscard]] unsigned int getFreqUpper() const;
        [[nodiscard]] unsigned int getSampleRes() const;
        [[nodiscard]] unsigned int getNumberOfSamples() const;
        std::vector<int>::const_iterator cbegin();
        std::vector<int>::const_iterator cend();
        std::vector<int>::iterator begin();
        std::vector<int>::iterator end();
        std::size_t size();
        friend std::ostream& operator<<(std::ostream &os, const Scan &scan);
    };
    typedef std::vector<std::string> string_vector;
    typedef std::vector<std::shared_ptr<Scan>> scan_vector;

    class ScanBuffer {
        std::deque<std::shared_ptr<Scan>> window;
        unsigned int max_length;
    public:
        void resize(unsigned int maxLength);
        [[nodiscard]] unsigned int getMaxLength() const;
        ScanBuffer();
        explicit ScanBuffer(unsigned int maxLength);
        unsigned int size();
        scan_vector getBufferView();
        void push_back(std::shared_ptr<Scan> scan);
    };


    typedef std::unordered_map<std::string, ScanBuffer> scan_storage_map;
    typedef boost::function<void(std::string&)> callback;

    class SpectoolsRaw {
        bp::child child;
        bp::ipstream child_stream;
        unsigned int buffer_depth = 1024;
        std::shared_ptr<string_vector> keys;
        std::shared_ptr<scan_storage_map> buffer;
        std::thread reader_thread;
        std::shared_ptr<std::vector<callback>> device_callbacks;

    public:
        SpectoolsRaw();
        //void resize();
        int join();
        unsigned int getMaxLength() const;
        const std::vector<std::string> &getKeys() const;
        ScanBuffer &getBuffer(std::string &key);
        void attach_listener(callback listener);
    };

}
#endif //SPECTOOLS_NG_SPECTOOL_WRAPPER_H
