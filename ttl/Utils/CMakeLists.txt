target_sources(Ttl
                PUBLIC
                    FILE_SET ttlUtilsHeaders
                            TYPE HEADERS
                            BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
                    FILES
                            LocklessFileAppender.h
                            LocklessWriterBase.h
                            LogTracer.h
                            MultipleInstanceTLS.h
                            MutexFileAppender.hpp
                            ScopedGuard.h
                            UnifiedResultCodes.h
                            Visitor.h
                            test_lockless.cpp
                            test_tls.cpp
                            CTimeUtils.h
                            LocklessFileAppender.hpp
                            LocklessWriterBase.hpp
                            LogTracerAdapter.h
                            MutexFileAppender.h
                            Pools/CommonObjectPool.h
                            Pools/LockPolicies.h
                            Pools/ObjectQueue.h
                            StringUtils.h
                            Utils.h
                            VisitorAcceptor.h
                            test_mutex.cpp
                )
