# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Git manager for DayZConfigMaster

"""
Git manager for DayZConfigMaster.

Uses GitPython library to provide Git integration.
"""

import os
from pathlib import Path
from typing import Dict, List, Optional, Any


class GitManager:
    """Manages Git operations for mod projects."""
    
    def __init__(self, projects_root: str):
        """
        Initialize with projects root path.
        
        Args:
            projects_root: Root directory containing mod projects
        """
        self.projects_root = Path(projects_root)
    
    def _get_repo(self, mod_name: str) -> Optional["git.Repo"]:
        """Get GitPython Repo object for a mod."""
        try:
            import git
            
            mod_dir = self.projects_root / "mods" / mod_name
            if not mod_dir.exists():
                return None
            
            repo_path = mod_dir
            return git.Repo(repo_path)
        
        except ImportError:
            print("GitPython not installed. Install with: pip install GitPython")
            return None
        
        except Exception as e:
            print(f"Error getting repo for {mod_name}: {e}")
            return None
    
    def init(self, mod_name: str) -> tuple:
        """Initialize a new Git repository."""
        try:
            import git
            
            mod_dir = self.projects_root / "mods" / mod_name
            if not mod_dir.exists():
                return False, f"Mod directory not found: {mod_dir}"
            
            if (mod_dir / ".git").exists():
                return False, "Repository already initialized"
            
            # Initialize repository
            repo = git.Repo.init(mod_dir)
            
            # Create .gitignore for DayZ mods
            gitignore_content = """
# Python cache
__pycache__/
*.py[cod]
*$py.class

# IDE files
.idea/
.vscode/
*.swp
*.swo

# Build outputs
*.pbo
*.pbi
*.bikinfi

# Private keys (NEVER commit these!)
*.biprivatekey
*.key

# Logs
*.log
server.log
DayZDiag_x64.log

# OS files
.DS_Store
Thumbs.db
"""
            
            gitignore_path = mod_dir / ".gitignore"
            gitignore_path.write_text(gitignore_content)
            
            # Initial commit
            repo.index.add(["."])
            repo.index.commit("Initial commit")
            
            return True, "Repository initialized"
        
        except ImportError:
            return False, "GitPython not installed. Install with: pip install GitPython"
        
        except Exception as e:
            return False, f"Failed to initialize repository: {str(e)}"
    
    def status(self, mod_name: str) -> Dict[str, Any]:
        """Get Git status for a mod."""
        repo = self._get_repo(mod_name)
        if not repo:
            return {"error": "Repository not found"}
        
        try:
            import git
            
            # Get diff stats
            staged = [d.a_path for d in repo.index.diff(repo.head.commit)]
            unstaged = repo.git.diff(name_only=True).splitlines()
            untracked = repo.untracked_files
            
            # Get branch info
            branch = repo.active_branch.name if not repo.head.is_detached else "detached"
            
            # Check if ahead/behind remote
            try:
                origin = repo.remotes.origin
                ahead = len(list(repo.iter_commits(f"origin/{branch}..{branch}")))
                behind = len(list(repo.iter_commits(f"{branch}..origin/{branch}")))
                
                has_remote = True
            except Exception:
                ahead = 0
                behind = 0
                has_remote = False
            
            return {
                "ok": True,
                "branch": branch,
                "ahead": ahead,
                "behind": behind,
                "dirty": bool(staged or unstaged or untracked),
                "has_remote": has_remote,
                "staged": staged,
                "unstaged": unstaged,
                "untracked": untracked
            }
        
        except Exception as e:
            return {"ok": False, "error": str(e)}
    
    def commit(self, mod_name: str, message: str, stage_all: bool = True) -> tuple:
        """Commit changes to the repository."""
        repo = self._get_repo(mod_name)
        if not repo:
            return False, f"Repository not found for {mod_name}"
        
        try:
            import git
            
            if stage_all:
                repo.index.add(["."])
            
            # Create commit
            repo.index.commit(message)
            
            return True, "Changes committed"
        
        except Exception as e:
            return False, f"Commit failed: {str(e)}"
    
    def push(self, mod_name: str) -> tuple:
        """Push changes to remote repository."""
        repo = self._get_repo(mod_name)
        if not repo:
            return False, f"Repository not found for {mod_name}"
        
        try:
            # Find remote
            origin = None
            for remote in repo.remotes:
                if remote.name == "origin":
                    origin = remote
                    break
            
            if not origin:
                return False, "No remote configured"
            
            # Push to remote
            origin.push()
            
            return True, "Pushed to remote"
        
        except Exception as e:
            return False, f"Push failed: {str(e)}"
    
    def pull(self, mod_name: str) -> tuple:
        """Pull changes from remote repository."""
        repo = self._get_repo(mod_name)
        if not repo:
            return False, f"Repository not found for {mod_name}"
        
        try:
            origin = None
            for remote in repo.remotes:
                if remote.name == "origin":
                    origin = remote
                    break
            
            if not origin:
                return False, "No remote configured"
            
            origin.pull()
            
            return True, "Pulled from remote"
        
        except Exception as e:
            return False, f"Pull failed: {str(e)}"
    
    def create_branch(self, mod_name: str, branch_name: str) -> tuple:
        """Create a new branch."""
        repo = self._get_repo(mod_name)
        if not repo:
            return False, f"Repository not found for {mod_name}"
        
        try:
            import git
            
            # Create and checkout branch
            repo.create_head(branch_name)
            
            return True, f"Branch '{branch_name}' created"
        
        except Exception as e:
            return False, f"Failed to create branch: {str(e)}"
    
    def list_branches(self, mod_name: str) -> List[str]:
        """List all branches for a repository."""
        repo = self._get_repo(mod_name)
        if not repo:
            return []
        
        try:
            return [b.name for b in repo.branches]
        
        except Exception as e:
            print(f"Error listing branches: {e}")
            return []
    def publish_repo(self, mod_name: str, github_owner: str, repo_name: str,
                     private: bool = False, description: Optional[str] = None) -> tuple:
        """
        Publish a mod's repository to GitHub.

        Requires the 'gh' CLI tool to be installed and authenticated.
        Creates a new GitHub repo and pushes the local repository to it.

        Args:
            mod_name: Name of the mod
            github_owner: GitHub username or organization
            repo_name: Name for the GitHub repository
            private: Whether to make the repo private (default: public)
            description: Optional description for the repository

        Returns:
            Tuple of (success, message)
        """
        import subprocess

        mod_dir = self.projects_root / "mods" / mod_name
        if not mod_dir.exists():
            return False, f"Mod directory not found: {mod_dir}"

        # Check if gh CLI is available
        try:
            result = subprocess.run(
                ['gh', '--version'],
                capture_output=True,
                text=True,
                timeout=10
            )
            if result.returncode != 0:
                return False, "GitHub CLI (gh) not installed or not working"
        except FileNotFoundError:
            return False, "GitHub CLI (gh) not found. Install from https://cli.github.com/"
        except subprocess.TimeoutExpired:
            return False, "GitHub CLI check timed out"

        # Check if the local repo has been initialized
        if not (mod_dir / ".git").exists():
            # Try to initialize if not done
            try:
                import git
                repo = git.Repo.init(mod_dir)
                repo.index.add(["."])
                repo.index.commit("Initial commit")
            except Exception as e:
                return False, f"Failed to initialize git repository: {e}"

        # Check if there's already a remote configured
        try:
            import git
            repo = git.Repo(mod_dir)

            # Remove existing origin if pointing to different URL
            for r in repo.remotes:
                if r.name == "origin":
                    try:
                        current_url = r.url
                        expected_url = f"https://github.com/{github_owner}/{repo_name}.git"
                        if current_url != expected_url:
                            repo.delete_remote(r)
                    except Exception:
                        pass

            # Add origin if not present
            if not any(r.name == "origin" for r in repo.remotes):
                repo.create_remote("origin", f"https://github.com/{github_owner}/{repo_name}.git")

        except ImportError:
            return False, "GitPython not installed. Install with: pip install GitPython"
        except Exception as e:
            return False, f"Failed to configure remote: {e}"

        # Create the GitHub repository using gh CLI
        args = [
            'gh', 'repo', 'create',
            f"{github_owner}/{repo_name}",
            "--source=.",
            "--remote=origin",
            "--push"
        ]

        if private:
            args.append("--private")
        else:
            args.append("--public")

        if description:
            args.extend(["--description", description])

        try:
            result = subprocess.run(
                args,
                cwd=str(mod_dir),
                capture_output=True,
                text=True,
                timeout=60
            )

            if result.returncode != 0:
                # Repository might already exist, just push to it
                try:
                    import git
                    repo = git.Repo(mod_dir)
                    origin = repo.remotes.origin
                    origin.push()
                    return True, f"Repository pushed to GitHub: https://github.com/{github_owner}/{repo_name}"
                except Exception:
                    pass

                return False, f"GitHub repository creation failed: {result.stderr.strip()}"

            return True, f"GitHub repository created and pushed: https://github.com/{github_owner}/{repo_name}"

        except subprocess.TimeoutExpired:
            return False, "GitHub command timed out"
        except Exception as e:
            return False, f"Failed to publish to GitHub: {e}"

    def create_release(self, mod_name: str, tag: str, title: Optional[str] = None,
                       notes: Optional[str] = None, prerelease: bool = False) -> tuple:
        """
        Create a GitHub release for a mod.

        Requires the 'gh' CLI tool to be installed and authenticated.
        Creates and pushes a Git tag, then creates a GitHub release at that tag.

        Args:
            mod_name: Name of the mod
            tag: Git tag name (e.g., "v1.0.0")
            title: Optional title for the release
            notes: Optional release notes/description
            prerelease: Whether to mark as pre-release

        Returns:
            Tuple of (success, message)
        """
        import subprocess

        mod_dir = self.projects_root / "mods" / mod_name
        if not mod_dir.exists():
            return False, f"Mod directory not found: {mod_dir}"

        # Check if gh CLI is available
        try:
            result = subprocess.run(
                ['gh', '--version'],
                capture_output=True,
                text=True,
                timeout=10
            )
            if result.returncode != 0:
                return False, "GitHub CLI (gh) not installed or not working"
        except FileNotFoundError:
            return False, "GitHub CLI (gh) not found. Install from https://cli.github.com/"
        except subprocess.TimeoutExpired:
            return False, "GitHub CLI check timed out"

        # Tag the current commit
        try:
            import git
            repo = git.Repo(mod_dir)

            # Check if tag already exists
            existing_tags = [t.name for t in repo.tags]
            if tag not in existing_tags:
                repo.create_tag(tag, message=f"Release {tag}")

            # Push the tag to remote
            origin = None
            for r in repo.remotes:
                if r.name == "origin":
                    origin = r
                    break

            if origin:
                origin.push(tag)
            else:
                return False, "No remote configured. Publish repo first."

        except ImportError:
            return False, "GitPython not installed. Install with: pip install GitPython"
        except Exception as e:
            return False, f"Failed to create tag: {e}"

        # Create the GitHub release using gh CLI
        args = [
            'gh', 'release', 'create',
            tag,
            "--title", title or tag
        ]

        if notes:
            args.extend(["--notes", notes])

        if prerelease:
            args.append("--prerelease")

        try:
            result = subprocess.run(
                args,
                cwd=str(mod_dir),
                capture_output=True,
                text=True,
                timeout=60
            )

            if result.returncode != 0:
                return False, f"GitHub release creation failed: {result.stderr.strip()}"

            release_url = f"https://github.com/{mod_name.split('/')[0] if '/' in mod_name else 'owner'}/{mod_name.split('/')[-1] if '/' in mod_name else mod_name}/releases/tag/{tag}"
            return True, f"GitHub release created: {release_url}"

        except subprocess.TimeoutExpired:
            return False, "GitHub command timed out"
        except Exception as e:
            return False, f"Failed to create GitHub release: {e}"
