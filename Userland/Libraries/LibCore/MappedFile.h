/*
 * Copyright (c) 2018-2021, Andreas Kling <kling@serenityos.org>
 * Copyright (c) 2023, kleines Filmröllchen <filmroellchen@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Error.h>
#include <AK/Noncopyable.h>
#include <AK/NonnullOwnPtr.h>
#include <AK/RefCounted.h>
#include <LibCore/Forward.h>

namespace Core {

class MappedFile {
    AK_MAKE_NONCOPYABLE(MappedFile);
    AK_MAKE_NONMOVABLE(MappedFile);

public:
    static ErrorOr<NonnullOwnPtr<MappedFile>> map(StringView path);
    static ErrorOr<NonnullOwnPtr<MappedFile>> map_from_file(NonnullOwnPtr<Core::File>, StringView path);
    static ErrorOr<NonnullOwnPtr<MappedFile>> map_from_fd_and_close(int fd, StringView path);
    ~MappedFile();

    void* data() { return m_data; }
    void const* data() const { return m_data; }

    size_t size() const { return m_size; }

    ReadonlyBytes bytes() const { return { m_data, m_size }; }

private:
    explicit MappedFile(void*, size_t);

    void* m_data { nullptr };
    size_t m_size { 0 };
};

class SharedMappedFile : public RefCounted<SharedMappedFile> {
public:
    explicit SharedMappedFile(NonnullOwnPtr<MappedFile> file)
        : m_file(move(file))
    {
    }

    MappedFile const& operator->() const { return *m_file; }
    MappedFile& operator->() { return *m_file; }

private:
    NonnullOwnPtr<MappedFile> m_file;
};

}
