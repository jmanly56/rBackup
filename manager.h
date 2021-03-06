/*
        Copyright Jonathan Manly 2020

        This file is part of rBackup.

        rBackup is free software: you can redistribute it and/or modify
        it under the terms of the GNU Lesser General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        rBackup is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public License
        along with rBackup.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MANAGER_H
#define MANAGER_H

#include "backupjob.h"
#include "utility.h"
#include <QJsonObject>
#include <QVariant>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <unordered_map>

class Manager
{
    public:
        Manager();
        ~Manager() = default;
        Manager(const Manager &) = delete;
        Manager &operator=(const Manager &) = delete;
        Manager(Manager &&) = default;
        Manager &operator=(Manager &&) = default;

        /*!
         * \brief Save all jobs to json.
         * \return 0 for success, -1 for failure.
         */
        int save_jobs();

        /*!
         * \brief Loads the backups into the manager.
         * \return 0 for success, -1 for failure;
         */
        int load_jobs();

        /*!
         * \brief Adds the given job to the jobs map.
         * \param Creates a copy of the BackupJob passed to it.
         * \return 0 for success, -1 for failure.
         */
        int add_new_job(BackupJob job);

        /*!
         * \brief Gets all the names of the jobs.
         * \return List of names.
         */
        std::list<std::string> get_job_names() const;

        /*!
         * \brief Looks for the given job
         * \param QString name of the job to locate.
         * \return The located job string.
         */
        QString get_job_text(QString name);

        /*!
         * \brief Update a job in the job's list.
         * \param job
         * \return
         */
        int update_job(BackupJob job);

        /*!
         * \brief Retrieves the given job.
         * \param Name of the job to retrieve.
         * \return Const Ref to the requested BackupJob.
         */
        const BackupJob &get_job(const std::string &name);

        /*!
         * \brief Enables the named job
         * \param Name of job to enable.
         * \return 0 for success, -1 for not found.
         */
        int enable_job(const QString &name);

        /*!
         * \brief Disables the named job.
         * \param Name of the job to disable
         * \return 0 for success, -1 for not found.
         */
        int disable_job(const QString &name);

        /*!
         * \brief Runs the given job.
         * \param Name of the job to run.
         * \return 0 on succes, -1 on failure.
         */
        int run_job(const QString &name);

        /*!
         * \brief Creates the objects for backups.
         * \param Name of the job to create objects for.
         * \return 0 for success, -1 for failure.
         */
        int create_systemd_objects(const QString &name);

        /*
         * \brief Delete job.
         * \param Name of job to delete.
         * \return 0 for success, -1 for failure.
         */ 
        int delete_job(const QString &name);

    private:
        /*
         * Uses unordered map to easily make sure there are no duplicates.
         */
        std::unordered_map<std::string, BackupJob> jobs;

        // Service path.
        QString servicePath;

        // Configuration file path.
        QString configPath;

        // Backup file path
        QString backupPath;

        /*!
         * \brief Creates a Json object of a job.
         * \param BackupJob to serialize.
         * \return Serialized BackupJob.
         */
        QJsonObject job_to_json(const BackupJob &job) const;

        /*!
         * \brief Creates an object containing the days to run on.
         * \param BackupJob containg days array to serialize.
         * \return QJsonObject to add to the main object.
         */
        QJsonObject days_to_json(const BackupJob &job) const;

        /*!
         * \brief Creates an object containing the job flags.
         * \param BackupJob containg JobFlags struct to serialize.
         * \return QJsonObject to add to the main object.
         */
        QJsonObject jobflags_to_json(const BackupJob &job) const;

        /*!
         * \brief Gets the home directory to store the json in.
         * \return The home directory.
         */
        QString find_home_directory() const;

        /*!
         * \brief Creates a job from json.
         * \param QJsonObject containing the job.
         * \return BackupJob to be saved into the map.
         */
        BackupJob job_from_json(const QJsonObject &json) const;

        /*!
         * \brief Create a days array from the json file.
         * \param QJsonObject containing the days data.
         * \return Days array.
         */
        Days days_from_json(const QJsonObject &json) const;

        /*!
         * \brief Creates the proper flags from json.
         * \param QJsonObject containing the flags.
         * \return JobFlags set with correct values.
         */
        JobFlags jobflags_from_json(const QJsonObject &json) const;

        /*!
         * \brief Loads the given json document.
         * \param Path to the json file.
         * \return QJsonObject containing the request file, or an empty one if not found.
         */
        QJsonObject load_json_document(const QString &path);
};

#endif // MANAGER_H
